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


void JsonObj_fprint(JsonObj* this, FILE* fp){
    #define print(x) fprintf(fp, x)
    assert(this->type == Dict);
}
