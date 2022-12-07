#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Json.h"
#include "../List/List.h"
#include "../HT/HashTable.h"

struct JsonObj {
    JsonType type;
    union {
        char* str;
        double num;
        int bool;
        List* array;
        HashTable* dict;
    };
};

struct JArrayIter {
    ListIterator* list_iter;
};

struct JDictIter {
    HTPairIterator* htp_iter;
    JDictPair* jd_pair;
};

// some declarations to enable static function indirect recursion
static inline void JsonObj_print(const JsonObj* this, FILE* fp, uint depth);
static JsonObj* parse_unknown(const char** sp);

JsonObj* JsonObj_new_string(const char* str){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    this->type = JString;
    this->str = malloc((strlen(str) + 1) * sizeof(char));
    if(!this->str)
        return NULL;
    strcpy(this->str, str);
    return this;
}

JsonObj* JsonObj_new_number(double num){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    this->type = JNumber;
    this->num = num;
    return this;
}

JsonObj* JsonObj_new_bool(int boolean){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    this->type = JBool;
    this->bool = boolean;
    return this;
}

JsonObj* JsonObj_new_array(void){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    List* list = List_new();
    if(!list){
        free(this);
        return NULL;
    } 
    this->type = JArray;
    this->array = list;
    return this;
}

JsonObj* JsonObj_new_dict(void){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    HashTable* ht = HashTable_new_init_size(8);
    if(!ht){
        free(this);
        return NULL;
    }
    this->type = JDict;
    this->dict = ht;
    return this;
}

JsonObj* JsonObj_new_null(){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    this->type = JNull;
    return this;
}

void JsonObj_destroy(JsonObj* this){
    if(!this)
        return;
    switch (this->type){
    case JString: {
        free(this->str);
        free(this);
        break;
    }
        
    case JNumber:
    case JBool: 
    case JNull: {
        free(this);
        break;
    }
        
    case JArray: {
        List_destroy(this->array);
        free(this);
        break;
    }
        
    case JDict: {
        HashTable_destroy(this->dict);
        free(this);
        break;
    }
        
    default:
        assert(0);
    }
}

void JsonObj_deep_destroy(JsonObj* this){
    switch (this->type){
    case JString:
    case JNumber:
    case JBool: 
    case JNull: {
        JsonObj_destroy(this);
        break;
    }

    case JArray: {
        List_map(this->array, (void (*)(void* ))JsonObj_deep_destroy);
        JsonObj_destroy(this);
        break;
    }

    case JDict: {
        HashTable_map(this->dict, (void (*)(void* ))JsonObj_deep_destroy);
        JsonObj_destroy(this);
        break;
    }

    default:
        assert(0);
    }
}


int JsonObj_array_append(JsonObj* jarray, JsonObj* elem){
    assert(jarray->type == JArray);
    return List_append(jarray->array, elem);
}

int JsonObj_array_insert(JsonObj* jarray, JsonObj* elem, unsigned int index){
    assert(jarray->type == JArray);
    return List_insert(jarray->array, elem, index);
}

int JsonObj_array_elem_deep_destroy(JsonObj* jarray, unsigned int index){
    assert(jarray->type == JArray);
    JsonObj* removed = List_remove(jarray->array, index);
    if(!removed)
        return -1;
    JsonObj_deep_destroy(removed);
    return 0;
}

int JsonObj_array_elem_destroy(JsonObj* jarray, unsigned int index){
    assert(jarray->type == JArray);
    JsonObj* removed = List_remove(jarray->array, index);
    if(!removed)
        return -1;
    JsonObj_destroy(removed);
    return 0;
}

void JsonObj_array_deep_clear(JsonObj* jarray){
    assert(jarray->type == JArray);
    List_map(jarray->array, (void (*)(void *))JsonObj_deep_destroy);
    List_clear(jarray->array);
}

void JsonObj_array_clear(JsonObj* jarray){
    assert(jarray->type == JArray);
    List_clear(jarray->array);
}

void JsonObj_array_map(JsonObj* jarray, void (*func)(JsonObj* )){
    assert(jarray->type == JArray);
    List_map(jarray->array, (void (*)(void*))func);
}

int JsonObj_dict_add(JsonObj* jdict, const char* key, JsonObj* value){
    assert(jdict->type == JDict);
    return HashTable_insert(jdict->dict, key, value);
}

int JsonObj_dict_elem_deep_destroy(JsonObj* jdict, const char* key){
    assert(jdict->type == JDict);
    JsonObj* removed = HashTable_remove(jdict->dict, key);
    if(!removed)
        return -1;
    JsonObj_deep_destroy(removed);
    return 0;
}

int JsonObj_dict_elem_destroy(JsonObj* jdict, const char* key){
    assert(jdict->type == JDict);
    JsonObj* removed = HashTable_remove(jdict->dict, key);
    if(!removed)
        return -1;
    JsonObj_destroy(removed);
    return 0;
}

void JsonObj_dict_deep_clear(JsonObj* jdict){
    assert(jdict->type == JDict);
    HTPairIterator* iter = HTPairIterator_new(jdict->dict);
    HTPair* pair;
    while((pair = HTPairIterator_next(iter)) != NULL)
        JsonObj_deep_destroy((JsonObj*)pair->value);
    HTPairIterator_destroy(iter);
    HashTable_clear(jdict->dict);
}

void JsonObj_dict_clear(JsonObj* jdict){
    assert(jdict->type == JDict);
    HashTable_clear(jdict->dict);
}

void JsonObj_dict_map(JsonObj* jdict, void (*func)(JsonObj* )){
    assert(jdict->type == JDict);
    JDictIter* iter = JDictIter_new(jdict);
    JDictPair* jdict_pair;
    while((jdict_pair = JDictIter_next(iter)) != NULL)
        func(jdict_pair->jsonobj);
    JDictIter_destroy(iter);
}

static inline void print_indentation(FILE* fp, uint depth){
    for(uint i=0; i<depth; i++)
        fprintf(fp, "    ");
}

static inline void JsonObj_str_print(const JsonObj* this, FILE* fp, uint depth){
    fprintf(fp, "\"%s\"", this->str);
}

static inline void JsonObj_num_print(const JsonObj* this, FILE* fp, uint depth){
    fprintf(fp, "%lf", this->num);
}

static inline void JsonObj_bool_print(const JsonObj* this, FILE* fp, uint depth){
    if(this->bool == 0)
        fprintf(fp, "false");
    else
        fprintf(fp, "true");
}

static inline void JsonObj_list_print(const JsonObj* this, FILE* fp, uint depth){
    ListIterator* iter = ListIterator_new(this->array);
    if(!ListIterator_peak(iter)) // if list empty
        fprintf(fp, "[ ]");
    else {
        fprintf(fp, "[\n");
        JsonObj* list_item;
        while(ListIterator_has_next(iter)){
            list_item = ListIterator_next(iter);
            print_indentation(fp, depth + 1);
            JsonObj_print(list_item, fp, depth + 1);
            fprintf(fp, ",\n"); 
        }
        list_item = ListIterator_next(iter);
        print_indentation(fp, depth + 1);
        JsonObj_print(list_item, fp, depth + 1);
        fprintf(fp, "\n");
        print_indentation(fp, depth);
        fprintf(fp, "]"); 
    }
    ListIterator_destroy(iter);
}

static inline void JsonObj_dict_print(const JsonObj* this, FILE* fp, uint depth){
    HTPairIterator* iter = HTPairIterator_new(this->dict);
    if(!HTPairIterator_peak(iter))
        fprintf(fp, "{}");
    else {
        fprintf(fp, "{\n");
        HTPair* pair;
        uint total_pairs = HashTable_element_count(this->dict);
        uint pair_num = 1;
        while((pair = HTPairIterator_next(iter)) != NULL){
            print_indentation(fp, depth + 1);
            fprintf(fp, "\"%s\" : ", pair->key);
            JsonObj_print((JsonObj*)pair->value, fp, depth + 1);
            if(pair_num++ < total_pairs)
                fprintf(fp, ",\n");
            else
                fprintf(fp, "\n");
        }
        print_indentation(fp, depth);
        fprintf(fp, "}");
    }
    HTPairIterator_destroy(iter);
}

static inline void JsonObj_print(const JsonObj* this, FILE* fp, uint depth){
    switch (this->type){
    case JString: {
        JsonObj_str_print(this, fp, depth);
        break;
    }

    case JNumber: {
        JsonObj_num_print(this, fp, depth);
        break;
    }

    case JBool: {
        JsonObj_bool_print(this, fp, depth);
        break;
    }
    case JArray: {
        JsonObj_list_print(this, fp, depth);
        break;
    }

    case JDict: {
        JsonObj_dict_print(this, fp, depth);
        break;
    }

    case JNull: {
        fprintf(fp, "null");
        break;
    }

    default:
        assert(0);
    }
}


void JsonObj_fprint(const JsonObj* this, FILE* fp){
    assert(this->type == JDict);
    JsonObj_dict_print(this, fp, 0);
    fprintf(fp, "\n");
}

char* JsonObj_sprint(const JsonObj* this){
    assert(0);
}

// after the call *sp points to the next 'non-empty char'
// if *sp is not an 'empty char' the the function does nothing
static inline void skip_empty(const char** sp){
    char c;
    while((c = **sp) != 0){
        switch(c){
        case ' ':
        case '\n':
        case '\t':
        case '\r':
            break; // break from switch

        default:
            return;
        }
        (*sp)++;
    }
}

// input: (*sp = "abcdef\"g") -> output (abcdef"g) [malloc'd]
// after the call *sp points to char after string the ending "
static char* parse_str(const char** sp){
    assert(**sp == '"');
    (*sp)++;
    const char* str_start = *sp;
    char c, prev = 0;
    uint len = 0;
    while((c = **sp) != 0){
        if((c == '\\') && (prev == '\\'))
            prev = 0;
        else if((c == '"') && (prev != '\\')){
            char* ret_val = malloc(sizeof(char) * (len + 1));
            if(!ret_val)
                return NULL;
            (*sp)++;
            strncpy(ret_val, str_start, len);
            ret_val[len] = 0;
            return ret_val;
        }
        len++;
        (*sp)++;
    }
    return NULL;
}

/* Used to read: Number, Bool, Null. 
*  Returns a malloc'd copy of the string starting at *sp and ending before one of the chars: 
*  ' '   '\t'   '\r'   '\n'   ','   '}' */
static char* read_token(const char** sp){
    const char* token_start = *sp;
    uint len = 0;
    char c;
    while((c = **sp) != 0){
        switch(c){
        case ' ':
        case '\t':
        case '\r':
        case '\n':
        case ',':
        case '}': {
            char* ret_val = malloc(sizeof(char) * (len + 1));
            if(!ret_val)
                return NULL;
            strncpy(ret_val, token_start, len);
            ret_val[len] = 0;
            return ret_val;
        }

        default: {
            len++;
            (*sp)++;
        }
        }
    }
    return NULL;
}

static JsonObj* JsonObj_parse_str(const char** sp){
    char* str = parse_str(sp);
    if(!str)
        return NULL;
    JsonObj* jstr = JsonObj_new_string(str);
    free(str);
    return jstr;
}

static JsonObj* JsonObj_parse_array(const char** sp){
    assert(**sp == '[');
    (*sp)++;
    JsonObj* jarray = JsonObj_new_array();
    if(!jarray)
        return NULL;
    char c;
    while(1){
        skip_empty(sp);
        c = **sp;
        switch(c){
        case ']': {
            (*sp)++;
            return jarray;
        }
        
        default: {
            JsonObj* json_obj = parse_unknown(sp);
            if(!json_obj){
                JsonObj_deep_destroy(jarray);
                return NULL;
            }
            JsonObj_array_append(jarray, json_obj);
            skip_empty(sp);
            if(**sp == ','){
                (*sp)++;
                break;
            }    
        }
        }
    }
}

static JsonObj* JsonObj_parse_bool_true(const char** sp){
    assert(**sp == 't');
    char* str = read_token(sp);
    if(!str)
        return NULL;
    if(strcmp(str, "true") != 0){
        free(str);
        return NULL;
    }
    free(str);
    return JsonObj_new_bool(1);
}

static JsonObj* JsonObj_parse_bool_false(const char** sp){
    assert(**sp == 'f');
    char* str = read_token(sp);
    if(!str)
        return NULL;
    if(strcmp(str, "false") != 0){
        free(str);
        return NULL;
    }
    free(str);
    return JsonObj_new_bool(0);
}

// returns 1 if successfully parse null. otherwise 0.
static JsonObj* JsonObj_parse_null(const char** sp){
    assert(**sp == 'n');
    char* str = read_token(sp);
    if(!str)
        return NULL;
    if(strcmp(str, "null") != 0){
        free(str);
        return NULL;
    }
    free(str);
    return JsonObj_new_null();
}

static JsonObj* JsonObj_parse_number(const char** sp){
    char* str = read_token(sp);
    if(!str)
        return NULL;
    char* after_last_parsed_char;
    double num = strtod(str, &after_last_parsed_char);
    uint str_len = strlen(str);
    if((str_len == 0) || (after_last_parsed_char != str + str_len)){
        free(str);
        return NULL;
    }
    free(str);
    return JsonObj_new_number(num);
}

static JsonObj* JsonObj_parse_dict(const char** sp){
    assert(**sp == '{');
    (*sp)++;
    JsonObj* jdict = JsonObj_new_dict();
    if(!jdict)
        return NULL;
    char c;
    char* key;
    while(1){
        skip_empty(sp);
        c = **sp;
        switch(c){
        case '"': {
            key = parse_str(sp);
            if(!key){
                JsonObj_deep_destroy(jdict);
                return NULL;
            }
            skip_empty(sp);
            if (**sp != ':'){
                free(key);
                JsonObj_deep_destroy(jdict);
                return NULL;
            }
            (*sp)++;
            skip_empty(sp);
            JsonObj* value = parse_unknown(sp);
            if(!value){
                free(key);
                JsonObj_deep_destroy(jdict);
                return NULL;
            }
            JsonObj_dict_add(jdict, key, value);
            free(key);
            skip_empty(sp);
            if(**sp == ','){
                (*sp)++;
                break;
            }
            if(**sp == '}'){
                (*sp)++;
                return jdict;
            }
            else
                return NULL;
        }

        default: {
            JsonObj_deep_destroy(jdict);
            return NULL;
        }
        }
    }
}

static JsonObj* parse_unknown(const char** sp){
    skip_empty(sp);
    char c = **sp;
    switch (c){
    case '{': { // obj start
        return JsonObj_parse_dict(sp);
    }

    case '"': { // string start
        return JsonObj_parse_str(sp);
    }

    case '[': { // array start
        return JsonObj_parse_array(sp);
    }

    case 't': { // boolean true start
        return JsonObj_parse_bool_true(sp);
    }

    case 'f': { // boolean false start
        return JsonObj_parse_bool_false(sp);
    }

    case 'n': { // null start
        return JsonObj_parse_null(sp);
    }
    
    default: { // number start or error
        return JsonObj_parse_number(sp);
    }
    }
}

JsonObj* JsonObj_parse_string(const char* sp){
    const char** sp_ref = &sp;
    JsonObj* this = parse_unknown(sp_ref);
    if(!this)
        return NULL;
    if(this->type != JDict){
        JsonObj_deep_destroy(this);
        return NULL;
    }
    return this;
}

JsonObj* JsonObj_parse_file(const char* json_file_path){
    FILE* json_fp = fopen(json_file_path, "r");
    if(!json_fp)
        return NULL;
    char* str;
    uint len;
    fseek(json_fp, 0, SEEK_END);
    len = ftell(json_fp);
    str = malloc(len + 1);
    if(!str){
        fclose(json_fp);
        return NULL;
    }
    fseek(json_fp, 0, SEEK_SET); // return to start of file
    fread(str, 1, len, json_fp);
    str[len] = '\0';
    fclose(json_fp);
    JsonObj* this = JsonObj_parse_string(str);
    free(str);
    return this;
}



/* JsonObj iterators & geters */
JsonType JsonObj_get_type(const JsonObj* this){
    return this->type;
}

const char* JsonObj_get_string(const JsonObj* jstring){
    assert(jstring->type == JString);
    return jstring->str;
}

double JsonObj_get_number(const JsonObj* jnumber){
    assert(jnumber->type == JNumber);
    return jnumber->num;
}

int JsonObj_get_bool(const JsonObj* jbool){
    assert(jbool->type == JBool);
    return jbool->bool;
}

unsigned int JsonObj_array_size(const JsonObj* jarray){
    assert(jarray->type == JArray);
    return List_length(jarray->array);
}

JsonObj* JsonObj_array_get(const JsonObj* jarray, unsigned int index){
    assert(jarray->type == JArray);
    return List_get(jarray->array, index);
}

unsigned int JsonObj_dict_size(const JsonObj* jdict){
    assert(jdict->type == JDict);
    return HashTable_element_count(jdict->dict);
}

JsonObj* JsonObj_dict_get(const JsonObj* jdict, const char* key){
    assert(jdict->type == JDict);
    return HashTable_get(jdict->dict, key);
}

/* JArrayIter functions */
JArrayIter* JArrayIter_new(const JsonObj* jarray){
    assert(jarray->type == JArray);
    JArrayIter* this = malloc(sizeof(JArrayIter));
    if(!this)
        return NULL;
    this->list_iter = ListIterator_new(jarray->array);
    if(!this->list_iter){
        free(this);
        return NULL;
    }
    return this;
}

JsonObj* JArrayIter_peak(JArrayIter* this){
    return ListIterator_peak(this->list_iter);
}

JsonObj* JArrayIter_next(JArrayIter* this){
    return ListIterator_next(this->list_iter);
}

void JarrayIter_reset(JArrayIter* this){
    ListIterator_reset(this->list_iter);
}

void JarrayIter_destroy(JArrayIter* this){
    ListIterator_destroy(this->list_iter);
    free(this);
}

/* JDictIter */
JDictIter* JDictIter_new(const JsonObj* jdict){
    assert(jdict->type == JDict);
    JDictIter* this = malloc(sizeof(JDictIter));
    if(!this)
        return NULL;
    this->htp_iter = HTPairIterator_new(jdict->dict);
    if(!this->htp_iter){
        free(this);
        return NULL;
    }
    return this;
}

JDictPair* JDictIter_peak(JDictIter* this){
    if(!this->jd_pair){
        HTPair* pair = HTPairIterator_peak(this->htp_iter);
        if(!pair)
            return NULL;
        this->jd_pair = malloc(sizeof(JDictPair));
        if(!this->jd_pair)
            return NULL;
        this->jd_pair->key = pair->key;
        this->jd_pair->jsonobj = (JsonObj*)pair->value;
    }
    return this->jd_pair;
}

JDictPair* JDictIter_next(JDictIter* this){
    HTPair* pair = HTPairIterator_next(this->htp_iter);
    if(!pair)
        return NULL;
    if(this->jd_pair)
        free(this->jd_pair);
    this->jd_pair = malloc(sizeof(JDictPair));
    this->jd_pair->key = pair->key;
    this->jd_pair->jsonobj = (JsonObj*)pair->value;
    return this->jd_pair;
}

void JDictIter_reset(JDictIter* this){
    HTPairIterator_reset(this->htp_iter);
    if(this->jd_pair)
        free(this->jd_pair);
    this->jd_pair = NULL;
}

void JDictIter_destroy(JDictIter* this){
    HTPairIterator_destroy(this->htp_iter);
    if(this->jd_pair)
        free(this->jd_pair);
    free(this);
}