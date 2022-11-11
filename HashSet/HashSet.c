#include "HashSet.h"
#include <string.h>
#include <stdlib.h>

typedef struct Node Node;

const uint32_t DEF_SIZE = 509;
const double DEF_MAX_LOAD_FACTOR = 0.7;

struct Node {
    void* data;
    int32_t deleted;
};

struct HashSet {
    uint32_t table_size;
    uint32_t element_count;
    uint32_t taken_spaces;
    double max_load_factor;
    Node **table;
    uint32_t (*hash_function)(const void* , uint32_t);
};

struct HSIterator {
    HashSet* set;
    uint32_t index;
};

static uint32_t def_hash_function(const void* data, uint32_t table_size){
    static const uint32_t HASH_MUTLIPLIER = 65599;
    uint32_t hash_value = 0;
    const char* threshold = data + sizeof(data);
    while (data != threshold)
        hash_value = hash_value * HASH_MUTLIPLIER + *(char*)data++;
    return hash_value % table_size;
}

static Node* Node_new(void* data){
    Node* this = malloc(sizeof(Node));
    this->data = data;
    this->deleted = 0;
    return this;
}

static void HashSet_insert_node(HashSet* this, Node* node){
    uint32_t idx = this->hash_function(node->data, this->table_size);
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

static int32_t HashSet_double_size(HashSet* this){
    HashSet* new_ht = HashSet_new_init_size(this->table_size * 2);
    uint32_t table_size = this->table_size;
    uint32_t element_count = this->taken_spaces;
    uint32_t elements_visited = 0;
    Node** old_table = this->table;
    for(uint32_t i=0; (elements_visited<element_count) && (i<table_size); i++){
        if(old_table[i]){
            if(old_table[i]->deleted == 0)
                HashSet_insert_node(new_ht, old_table[i]);
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

HashSet* HashSet_new(void){
    return HashSet_new_init_size(DEF_SIZE);
}

HashSet* HashSet_new_init_size(uint32_t init_size){
    HashSet* this = malloc(sizeof(HashSet));
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

HashSet* HashSet_set_hash_function(HashSet* this, uint32_t (*new_hash_function)(const void* data, uint32_t table_size)) {
    this->hash_function = new_hash_function;
}

int32_t HashSet_set_max_load_factor(HashSet* this, double max_load_factor){
    if(max_load_factor >= 1.0 || max_load_factor <= 0)
        return -1;
    this->max_load_factor = max_load_factor;
    return 0;
}

double HashSet_get_max_load_factor(HashSet* this){
    return this->max_load_factor;
}

double HashSet_get_current_load_factor(HashSet* this){
    return (double)this->taken_spaces / (double)this->table_size;
}

void HashSet_destroy(HashSet* this){
    uint32_t table_size = this->table_size;
    uint32_t element_count = this->taken_spaces;
    uint32_t elements_visited = 0;
    Node** table = this->table;
    for(uint32_t i=0; (elements_visited<element_count) && (i<table_size); i++){
        if(table[i]){
            free(table[i]);
            elements_visited++;
        }
    }
    free(table);
    free(this);
}

void HashSet_clear(HashSet* this){
    uint32_t table_size = this->table_size;
    uint32_t element_count = this->taken_spaces;
    uint32_t elements_visited = 0;
    Node** table = this->table;
    for(uint32_t i=0; (elements_visited<element_count) && (i<table_size); i++){
        if(table[i]){
            if(table[i]->deleted == 0)
                free(table[i]->data);
            free(table[i]);
            table[i] = NULL;
            elements_visited++;
        }
    }
    this->element_count = 0;
    this->taken_spaces = 0;
}

uint32_t HashSet_capacity(HashSet* this){
    return this->table_size;
}

uint32_t HashSet_element_count(HashSet* this){
    return this->element_count;
}

static void** HashSet_get_val_ref(HashSet* this, const char* key){
    uint32_t idx = this->hash_function(key, this->table_size);
    uint32_t visited = 0;
    Node** table = this->table;
    while(visited < this->element_count){
        if(table[idx]){
            if((table[idx]->deleted == 0) && strcmp(key, table[idx]->data) == 0)
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

int32_t HashSet_insert(HashSet* this, void* data){
    if((double)this->taken_spaces / (double)this->table_size >= this->max_load_factor){
        if(HashSet_double_size(this) == -1)
            return -1;
    }
    void** val_ref = HashSet_get_val_ref(this, data);
    if (val_ref != NULL) {
        *val_ref = (void*)data;
        return 0;
    }
    Node* new_node = Node_new(data);
    if(!new_node)
        return -1;
    HashSet_insert_node(this, new_node);
    this->element_count++;
    this->taken_spaces++;
    return 0;
}

void* HashSet_remove(HashSet* this, void* key){
    uint32_t idx = this->hash_function(key, this->table_size);
    uint32_t visited = 0;
    Node** table = this->table;
    while(visited < this->element_count){
        if(table[idx]){
            if((table[idx]->deleted == 0) && strcmp(key, table[idx]->data) == 0){
                void* data = table[idx]->data;
                free(table[idx]->data);
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

int32_t HashSet_contains(HashSet* this, void* key){
    uint32_t idx = this->hash_function(key, this->table_size);
    uint32_t visited = 0;
    Node** table = this->table;
    while(visited < this->element_count){
        if(table[idx]){
            if((table[idx]->deleted == 0) && strcmp(key, table[idx]->data) == 0)
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

void HashSet_map(HashSet* this, void (*func)(void* )){
    HSIterator* iter = HSIterator_new(this);
    void* ht_elem;
    while(ht_elem = HSIterator_next(iter))
        func(ht_elem);
    HSIterator_destroy(iter);
}

HSIterator* HSIterator_new(HashSet* set){
    HSIterator* this = malloc(sizeof(HSIterator));
    if(!this)
        return NULL;
    this->set = set;
    this->index = 0;
    return this;
}

void HSIterator_destroy(HSIterator* this){
    free(this);
}

void* HSIterator_peak(HSIterator* this){
    Node* node;
    while(this->index < this->set->table_size){
        node = this->set->table[this->index];
        if((node != NULL) && (node->deleted == 0))
            return node->data;
        this->index++;
    }
    return NULL;
}

void* HSIterator_next(HSIterator* this){
    Node* node;
    while(this->index < this->set->table_size){
        node = this->set->table[this->index++];
        if((node != NULL) && (node->deleted == 0))
            return node->data;
    }
    return NULL;
}

void HSIterator_reset(HSIterator* this){
    this->index = 0;
}

bool _HSIterator_destroy(HSIterator* this) {
    HSIterator_destroy(this);
    return false;
}
