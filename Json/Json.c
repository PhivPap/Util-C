#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Json.h"
#include "../List/List.h"
#include "../HT/HashTable.h"

typedef enum Type { String, Number, Bool, _List, Dict} Type;

struct JsonObj {
    Type type;
    union {
        char* str;
        double num;
        int bool;
        List* list;
        HashTable* ht;
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

JsonObj* JsonObj_new_list(void){
    JsonObj* this = malloc(sizeof(JsonObj));
    List* list = List_new();
    if((!this) || (!list))
        return NULL;
    this->type = _List;
    this->list = list;
    return this;
}

JsonObj* JsonObj_new_dict(void){
    JsonObj* this = malloc(sizeof(JsonObj));
    HashTable* ht = HashTable_new_with_size(8);
    if((!this) || (!ht))
        return NULL;
    this->type = Dict;
    this->ht = ht;
    return this;
}


int JsonObj_list_append(JsonObj* this, JsonObj* elem){
    assert(this->type == _List);
    return List_append(this->list, elem);
}

int JsonObj_dict_add(JsonObj* this, const char* key, JsonObj* value){
    assert(this->type == Dict);
    return HashTable_insert(this->ht, key, value);
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
    ListIterator* iter = ListIterator_new(this->list);
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
    HTPairIterator* iter = HTPairIterator_new(this->ht);
    if(!HTPairIterator_peak(iter))
        fprintf(fp, "{ }\n");
    else {
        fprintf(fp, "{\n");
        HTPair* pair;
        uint total_pairs = HashTable_element_count(this->ht);
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

    case _List:
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
