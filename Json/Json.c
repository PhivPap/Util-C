#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Json.h"
#include "../List/List.h"
#include "../HT/HashTable.h"

typedef enum Type { String, Number, Bool, Array, Dict, Null } Type;

struct JsonObj {
    Type type;
    union {
        char* str;
        double num;
        int bool;
        List* array;
        HashTable* dict;
    };
};

// some declarations to enable static function indirect recursion
static inline void JsonObj_print(const JsonObj* this, FILE* fp, uint depth);
static JsonObj* parse_unknown(const char** sp);

JsonObj* JsonObj_new_string(const char* str){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    this->type = String;
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
    this->type = Number;
    this->num = num;
    return this;
}

JsonObj* JsonObj_new_bool(int boolean){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    this->type = Bool;
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
    this->type = Array;
    this->array = list;
    return this;
}

JsonObj* JsonObj_new_dict(void){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    HashTable* ht = HashTable_new_with_size(8);
    if(!ht){
        free(this);
        return NULL;
    }
    if((!this) || (!ht))
        return NULL;
    this->type = Dict;
    this->dict = ht;
    return this;
}

JsonObj* JsonObj_new_null(){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    this->type = Null;
    return this;
}

void JsonObj_destroy(JsonObj* this){
    if(!this)
        return;
    switch (this->type){
    case String: {
        free(this->str);
        free(this);
        break;
    }
        
    case Number:
    case Bool: 
    case Null: {
        free(this);
        break;
    }
        
    case Array: {
        List_destroy(this->array);
        free(this);
        break;
    }
        
    case Dict: {
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
    case String:
    case Number:
    case Bool: 
    case Null: {
        JsonObj_destroy(this);
        break;
    }

    case Array: {
        ListIterator* iter = ListIterator_new(this->array);
        JsonObj* list_item;
        while(list_item = ListIterator_next(iter))
            JsonObj_deep_destroy(list_item);
        ListIterator_destroy(iter);
        JsonObj_destroy(this);
        break;
    }

    case Dict: {
        HTIterator* iter = HTIterator_new(this->dict);
        JsonObj* dict_item;
        while(dict_item = HTIterator_next(iter))
            JsonObj_deep_destroy(dict_item);
        HTIterator_destroy(iter);
        JsonObj_destroy(this);
        break;
    }

    default:
        assert(0);
    }
}


int JsonObj_array_append(JsonObj* this, JsonObj* elem){
    assert(this->type == Array);
    return List_append(this->array, elem);
}

int JsonObj_dict_add(JsonObj* this, const char* key, JsonObj* value){
    assert(this->type == Dict);
    return HashTable_insert(this->dict, key, value);
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
        fprintf(fp, "{ }\n");
    else {
        fprintf(fp, "{\n");
        HTPair* pair;
        uint total_pairs = HashTable_element_count(this->dict);
        uint pair_num = 1;
        while(pair = HTPairIterator_next(iter)){
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
    case String: {
        JsonObj_str_print(this, fp, depth);
        break;
    }

    case Number: {
        JsonObj_num_print(this, fp, depth);
        break;
    }

    case Bool: {
        JsonObj_bool_print(this, fp, depth);
        break;
    }
    case Array: {
        JsonObj_list_print(this, fp, depth);
        break;
    }

    case Dict: {
        JsonObj_dict_print(this, fp, depth);
        break;
    }

    case Null: {
        fprintf(fp, "null");
        break;
    }

    default:
        assert(0);
    }
}


void JsonObj_fprint(const JsonObj* this, FILE* fp){
    assert(this->type == Dict);
    JsonObj_dict_print(this, fp, 0);
    fprintf(fp, "\n");
}

// after the call *sp points to the next 'non-empty char'
// if *sp is not an 'empty char' the the function does nothing
static inline void skip_empty(const char** sp){
    char c;
    while(c = **sp){
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

// static inline char* read_to_next_empty_init_char(FILE* fp, char init){
//     char c;
//     uint idx = 0, str_size = INIT_STR_LEN;
//     char* str = calloc(str_size, sizeof(char));
//     if(!str)
//         return NULL;
//     if(init)
//         str[idx++] = init;
//     while((c == getc(fp))!= EOF){
//         if((c == '\0') || (c == ' ') || (c == '\n') || (c == '\t') || (c == '\r'))
//             return str;
//         str[idx++] = c;
//         if(idx >= str_size){
//             str_size *= STR_LEN_MUL;
//             char* new_str = calloc(str_size, sizeof(char));
//             if(!new_str){
//                 free(str);
//                 return NULL;
//             }
//             strcpy(new_str, str);
//             free(str);
//             str = new_str;
//         }
//     }
//     return NULL;
// }

// static inline char* read_to_next_empty(FILE* fp){
//     read_to_next_empty_init_char(fp, 0);
// }

// static inline char* parse_str(const char** sp){
//     char c, prev = 0;
//     uint idx = 0, str_size = INIT_STR_LEN;
//     char* str = calloc(str_size, sizeof(char));
//     if(!str)
//         return NULL;
//     while(c = **sp){
//         // FIX "\\n"
//         if((c == '"') && (prev != '\\'))
//             return str;
//         str[idx++] = c;
//         prev = c;
//         if(idx + 1 >= str_size){
//             str_size *= STR_LEN_MUL;
//             char* new_str = calloc(str_size, sizeof(char));
//             if(!new_str){
//                 free(str);
//                 return NULL;
//             }
//             strcpy(new_str, str);
//             free(str);
//             str = new_str;
//         }
//     }
//     return NULL;
// }

// input: (*sp = "abcdef\"g") -> output (abcdef"g) [malloc'd]
// after the call *sp points to char after string the ending "
static char* parse_str(const char** sp){
    assert(**sp == '"');
    (*sp)++;
    const char* str_start = *sp;
    char c, prev = 0;
    uint len = 0;
    while(c = **sp){
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
    while(c = **sp){
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
            (*sp)++;
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
    while(1){
        skip_empty(sp);
        if(**sp == ']'){
            (*sp)++;
            return jarray;
        }
        JsonObj* json_obj = parse_unknown(sp);
        if(!json_obj){
            JsonObj_deep_destroy(jarray);
            return NULL;
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
    if(after_last_parsed_char != str + strlen(str)){
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
            JsonObj* value = parse_unknown(sp);
            if(!value){
                free(key);
                JsonObj_deep_destroy(jdict);
                return NULL;
            }
            JsonObj_dict_add(jdict, key, value);
            break;
        }
        
        case '}': {
            (*sp)++;
            return jdict;
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
    if(this->type != Dict){
        JsonObj_deep_destroy(this);
        return NULL;
    }
    return this;
}

JsonObj* JsonObj_parser(FILE* fp){
    // char c = ignore_empty_to_next_char(fp);
    // if(c != '{')
    //     return NULL;
    // else 
    //     return parse_dict(fp);
    // JsonObj* this = parse_unknown(fp);
    // if(!this)
    //     return NULL;
    // if(this->type != Dict){
    //     JsonObj_deep_destroy(this);
    //     return NULL;
    // }
    // return this;

    assert(0);
}