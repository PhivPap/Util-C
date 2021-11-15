#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Json.h"
#include "../List/List.h"
#include "../HT/HashTable.h"

enum Type { String, Number, Bool, _List, Dict};

typedef enum Type Type;
struct JsonObj {
    Type type;
    union Value {
        char* str;
        double num;
        int bool;
        List* list;
        HashTable* ht;
    } value;
};  

JsonObj* JsonObj_new_string(const char* str){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    this->type = String;
    this->value.str = malloc((strlen(str) + 1) * sizeof(char));
    if(!this->value.str)
        return NULL;
    strcpy(this->value.str, str);
    return this;
}

JsonObj* JsonObj_new_number(double num){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    this->type = Number;
    this->value.num = num;
    return this;
}

JsonObj* JsonObj_new_bool(int boolean){
    JsonObj* this = malloc(sizeof(JsonObj));
    if(!this)
        return NULL;
    this->type = Bool;
    this->value.bool = boolean;
    return this;
}

JsonObj* JsonObj_new_list(void){
    JsonObj* this = malloc(sizeof(JsonObj));
    List* list = List_new();
    if((!this) || (!list))
        return NULL;
    this->type = _List;
    this->value.list = list;
    return this;
}

JsonObj* JsonObj_new_dict(void){
    JsonObj* this = malloc(sizeof(JsonObj));
    HashTable* ht = HashTable_new_with_size(8);
    if((!this) || (!ht))
        return NULL;
    this->type = Dict;
    this->value.ht = ht;
    return this;
}


int JsonObj_list_append(JsonObj* this, JsonObj* elem){
    assert(this->type == _List);
    return List_append(this->value.list, elem);
}

int JsonObj_dict_add(JsonObj* this, const char* key, JsonObj* value){
    assert(this->type == Dict);
    return HashTable_insert(this->value.ht, key, value);
}


static inline void print_indentation(FILE* fp, uint depth){
    for(uint i=0; i<depth; i++)
        fprintf(fp, "    ");
}

static inline void JsonObj_str_print(JsonObj* this, FILE* fp, uint depth){
    fprintf(fp, "\"%s\"\n", this->value.str);
}

static inline void JsonObj_num_print(JsonObj* this, FILE* fp, uint depth){
    fprintf(fp, "%lf\n", this->value.num);
}

static inline void JsonObj_bool_print(JsonObj* this, FILE* fp, uint depth){
    if(this->value.bool == 0)
        fprintf(fp, "false\n");
    else
        fprintf(fp, "true\n");
}

static inline void JsonObj_list_print(JsonObj* this, FILE* fp, uint depth){
    
}

static inline void JsonObj_dict_print(JsonObj* this, FILE* fp, uint depth){
    fprintf("{\n");
    
    fprintf("}\n");
}


static inline void JsonObj_fprint_recursive(JsonObj* this, FILE* fp, uint depth){
    

void JsonObj_fprint(JsonObj* this, FILE* fp){
    assert(this->type == Dict);
    
}
