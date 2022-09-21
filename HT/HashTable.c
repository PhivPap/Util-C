#include "HashTable.h"
#include <string.h>
#include <stdlib.h>

typedef struct Node Node;

const uint32_t DEF_SIZE = 509;
const double DEF_MAX_LOAD_FACTOR = 0.7;

struct Node {
    char* key;
    void* data;
    int32_t deleted;
};

struct HashTable {
    uint32_t table_size;
    uint32_t element_count;
    uint32_t taken_spaces;
    double max_load_factor;
    Node **table;
    uint32_t (*hash_function)(const char* , uint32_t);
};

struct HTIterator {
    HashTable* hashtable;
    uint32_t index;
};

struct HTKeyIterator {
    HashTable* hashtable;
    uint32_t index;
};

struct HTPairIterator {
    HashTable* hashtable;
    uint32_t index;
    HTPair pair;
};

static uint32_t def_hash_function(const char* key, uint32_t table_size){
    static const uint32_t HASH_MUTLIPLIER = 65599;
    uint32_t hash_value = 0;
    while(*key)
        hash_value = hash_value * HASH_MUTLIPLIER + *key++;
    return hash_value % table_size;
}

static Node* Node_new(const char* key, const void* data){
    Node* this = malloc(sizeof(Node));
    this->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(this->key, key);
    this->data = (void*)data;
    this->deleted = 0;
    return this;
}

static void HashTable_insert_node(HashTable* this, Node* node){
    uint32_t idx = this->hash_function(node->key, this->table_size);
    Node** table = this->table;
    while(1){
        if(table[idx]){
            if(table[idx]->deleted == 1){
                free(table[idx]);
                table[idx] = node;
                break;
            }
        }
        else {
            table[idx] = node;
            break;
        }
        idx++;
        idx = idx % this->table_size;
    }
}

static int32_t HashTable_double_size(HashTable* this){
    HashTable* new_ht = HashTable_new_init_size(this->table_size * 2);
    uint32_t table_size = this->table_size;
    uint32_t element_count = this->taken_spaces;
    uint32_t elements_visited = 0;
    Node** old_table = this->table;
    for(uint32_t i=0; (elements_visited<element_count) && (i<table_size); i++){
        if(old_table[i]){
            if(old_table[i]->deleted == 0)
                HashTable_insert_node(new_ht, old_table[i]);
            else
                free(old_table[i]);
            elements_visited++;
        }
    }
    free(old_table);
    this->table = new_ht->table;
    this->table_size = new_ht->table_size;
    this->taken_spaces = this->element_count;
    free(new_ht);
    return 0;
}

HashTable* HashTable_new(void){
    return HashTable_new_init_size(DEF_SIZE);
}

HashTable* HashTable_new_init_size(uint32_t init_size){
    HashTable* this = malloc(sizeof(HashTable));
    if(!this) 
        return NULL;
    this->hash_function = def_hash_function;
    this->table_size = init_size;
    this->element_count = 0;
    this->taken_spaces = 0;
    this->max_load_factor = DEF_MAX_LOAD_FACTOR;
    this->table = calloc(init_size, sizeof(Node*));
    if(!this->table){
        free(this);
        return NULL;
    }
    return this;
}

HashTable* HashTable_set_hash_function(HashTable* this, uint32_t (*new_hash_function)(const char* key, uint32_t table_size)) {
    this->hash_function = new_hash_function;
}

int32_t HashTable_set_max_load_factor(HashTable* this, double max_load_factor){
    if(max_load_factor >= 1.0 || max_load_factor <= 0)
        return -1;
    this->max_load_factor = max_load_factor;
    return 0;
}

double HashTable_get_max_load_factor(HashTable* this){
    return this->max_load_factor;
}

double HashTable_get_current_load_factor(HashTable* this){
    return (double)this->taken_spaces / (double)this->table_size;
}

void HashTable_destroy(HashTable* this){
    uint32_t table_size = this->table_size;
    uint32_t element_count = this->taken_spaces;
    uint32_t elements_visited = 0;
    Node** table = this->table;
    for(uint32_t i=0; (elements_visited<element_count) && (i<table_size); i++){
        if(table[i]){
            if(table[i]->deleted == 0)
                free(table[i]->key);
            free(table[i]);
            elements_visited++;
        }
    }
    free(table);
    free(this);
}

void HashTable_clear(HashTable* this){
    uint32_t table_size = this->table_size;
    uint32_t element_count = this->taken_spaces;
    uint32_t elements_visited = 0;
    Node** table = this->table;
    for(uint32_t i=0; (elements_visited<element_count) && (i<table_size); i++){
        if(table[i]){
            if(table[i]->deleted == 0)
                free(table[i]->key);
            free(table[i]);
            table[i] = NULL;
            elements_visited++;
        }
    }
    this->element_count = 0;
    this->taken_spaces = 0;
}

uint32_t HashTable_capacity(HashTable* this){
    return this->table_size;
}

uint32_t HashTable_element_count(HashTable* this){
    return this->element_count;
}

static void** HashTable_get_val_ref(HashTable* this, const char* key){
    uint32_t idx = this->hash_function(key, this->table_size);
    uint32_t visited = 0;
    Node** table = this->table;
    while(visited < this->element_count){
        if(table[idx]){
            if((table[idx]->deleted == 0) && strcmp(key, table[idx]->key) == 0)
                return &(table[idx]->data);
        }
        else
            break;
        visited++;
        idx++;
        idx = idx % this->table_size;
    }
    return NULL;
}

int32_t HashTable_insert(HashTable* this, const char* key, const void* value){
    if((double)this->taken_spaces / (double)this->table_size >= this->max_load_factor){
        if(HashTable_double_size(this) == -1)
            return -1;
    }
    void** val_ref = HashTable_get_val_ref(this, key);
    if (val_ref != NULL) {
        *val_ref = (void*)value;
        return 0;
    }
    Node* new_node = Node_new(key, value);
    if(!new_node)
        return -1;
    HashTable_insert_node(this, new_node);
    this->element_count++;
    this->taken_spaces++;
    return 0;
}

void* HashTable_get(HashTable* this, const char* key){
    uint32_t idx = this->hash_function(key, this->table_size);
    uint32_t visited = 0;
    Node** table = this->table;
    while(visited < this->element_count){
        if(table[idx]){
            if((table[idx]->deleted == 0) && strcmp(key, table[idx]->key) == 0)
                return table[idx]->data;
        }
        else
            break;
        visited++;
        idx++;
        idx = idx % this->table_size;
    }
    return NULL;
}

void* HashTable_remove(HashTable* this, const char* key){
    uint32_t idx = this->hash_function(key, this->table_size);
    uint32_t visited = 0;
    Node** table = this->table;
    while(visited < this->element_count){
        if(table[idx]){
            if((table[idx]->deleted == 0) && strcmp(key, table[idx]->key) == 0){
                void* data = table[idx]->data;
                free(table[idx]->key);
                table[idx]->deleted = 1;
                this->element_count--;
                return data;
            }
        }
        else
            break;
        visited++;
        idx++;
        idx = idx % this->table_size;
    }
    return NULL;
}

int32_t HashTable_contains(HashTable* this, const char* key){
    uint32_t idx = this->hash_function(key, this->table_size);
    uint32_t visited = 0;
    Node** table = this->table;
    while(visited < this->element_count){
        if(table[idx]){
            if((table[idx]->deleted == 0) && strcmp(key, table[idx]->key) == 0)
                return 1;
        }
        else
            break;
        visited++;
        idx++;
        idx = idx % this->table_size;
    }
    return 0;
}

void HashTable_map(HashTable* this, void (*func)(void* )){
    HTIterator* iter = HTIterator_new(this);
    void* ht_elem;
    while(ht_elem = HTIterator_next(iter))
        func(ht_elem);
    HTIterator_destroy(iter);
}

HTIterator* HTIterator_new(HashTable* hashtable){
    HTIterator* this = malloc(sizeof(HTIterator));
    if(!this)
        return NULL;
    this->hashtable = hashtable;
    this->index = 0;
    return this;
}

void HTIterator_destroy(HTIterator* this){
    free(this);
}

void* HTIterator_peak(HTIterator* this){
    Node* node;
    while(this->index < this->hashtable->table_size){
        node = this->hashtable->table[this->index];
        if((node != NULL) && (node->deleted == 0))
            return node->data;
        this->index++;
    }
    return NULL;
}

void* HTIterator_next(HTIterator* this){
    Node* node;
    while(this->index < this->hashtable->table_size){
        node = this->hashtable->table[this->index++];
        if((node != NULL) && (node->deleted == 0))
            return node->data;
    }
    return NULL;
}

void HTIterator_reset(HTIterator* this){
    this->index = 0;
}

HTKeyIterator* HTKeyIterator_new(HashTable* hashtable) {
    HTKeyIterator* this = malloc(sizeof(HTKeyIterator));
    if(!this)
        return NULL;
    this->hashtable = hashtable;
    this->index = 0;
    return this;
}

void HTKeyIterator_destroy(HTKeyIterator* this) {
    free(this);
}

const char* HTKeyIterator_peak(HTKeyIterator* this) {
    Node* node;
    while(this->index < this->hashtable->table_size){
        node = this->hashtable->table[this->index];
        if((node != NULL) && (node->deleted == 0))
            return node->key;
        this->index++;
    }
    return NULL;
}

const char* HTKeyIterator_next(HTKeyIterator* this) {
    Node* node;
    while(this->index < this->hashtable->table_size){
        node = this->hashtable->table[this->index++];
        if((node != NULL) && (node->deleted == 0))
            return node->key;
    }
    return NULL;
}

void HTKeyIterator_reset(HTKeyIterator* this) {
    this->index = 0;
}

HTPairIterator* HTPairIterator_new(HashTable* hashtable){
    HTPairIterator* this = malloc(sizeof(HTPairIterator));
    if(!this)
        return NULL;
    this->hashtable = hashtable;
    this->index = 0;
    return this;
}

void HTPairIterator_destroy(HTPairIterator* this){
    free(this);
}

HTPair* HTPairIterator_peak(HTPairIterator* this){
    Node* node;
    while(this->index < this->hashtable->table_size){
        node = this->hashtable->table[this->index];
        if((node != NULL) && (node->deleted == 0)){
            this->pair.key = node->key;
            this->pair.value = node->data;
            return &(this->pair);
        }
        this->index++;
    }
    return NULL;
}

HTPair* HTPairIterator_next(HTPairIterator* this){
    Node* node;
    while(this->index < this->hashtable->table_size){
        node = this->hashtable->table[this->index++];
        if((node != NULL) && (node->deleted == 0)){
            this->pair.key = node->key;
            this->pair.value = node->data;
            return &(this->pair);
        }
    }
    return NULL;
}

void HTPairIterator_reset(HTPairIterator* this){
    this->index = 0;
}