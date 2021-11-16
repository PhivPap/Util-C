#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Json.h"
#include "../List/List.h"
#include "../HT/HashTable.h"

typedef enum Type { String, Number, Bool, Array, Dict} Type;

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
    List* list = List_new();
    if((!this) || (!list))
        return NULL;
    this->type = Array;
    this->array = list;
    return this;
}

JsonObj* JsonObj_new_dict(void){
    JsonObj* this = malloc(sizeof(JsonObj));
    HashTable* ht = HashTable_new_with_size(8);
    if((!this) || (!ht))
        return NULL;
    this->type = Dict;
    this->dict = ht;
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
    case Bool: {
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
    if(!this)
        return;
    switch (this->type){
    case String:
    case Number:
    case Bool: {
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

static inline char ignore_empty_to_next_char(FILE* fp){
    char reeeee;
    while((reeeee == getc(fp))!= EOF){
        if(reeeee != '\0' && reeeee != ' ' && reeeee != '\n' && reeeee != '\t' && reeeee != '\r')
            return reeeee;
    }
    return 0;
}



static char* parse_dict_key(FILE* fp){
    assert(0);
    return NULL;
}


static JsonObj* parse_dict(FILE* fp){
    assert(0);
    return NULL;
}

JsonObj* JsonObj_parser(FILE* fp){
    // char c = ignore_empty_to_next_char(fp);
    // if(c != '{')
    //     return NULL;
    // else 
    //     return parse_dict(fp);
    assert(0);
    return NULL;
    
}


int JsonObj_array_append(JsonObj* this, JsonObj* elem){
    assert(this->type == Array);
    return List_append(this->array, elem);
}

int JsonObj_dict_add(JsonObj* this, const char* key, JsonObj* value){
    assert(this->type == Dict);
    return HashTable_insert(this->dict, key, value);
}


static inline void JsonObj_print(const JsonObj* this, FILE* fp, uint depth);

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
    if(!this){
        fprintf(fp, "null");
        return;
    }
    switch (this->type){
    case String:
        JsonObj_str_print(this, fp, depth);
        return;

    case Number:
        JsonObj_num_print(this, fp, depth);
        return;

    case Bool:
        JsonObj_bool_print(this, fp, depth);
        return;

    case Array:
        JsonObj_list_print(this, fp, depth);
        return;

    case Dict:
        JsonObj_dict_print(this, fp, depth);
        return;
    
    default:
        assert(0);
    }
}


void JsonObj_fprint(const JsonObj* this, FILE* fp){
    assert(this->type == Dict);
    JsonObj_dict_print(this, fp, 0);
    fprintf(fp, "\n");
}
