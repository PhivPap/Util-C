#include <stdlib.h>
#include "Vector.h"

typedef unsigned int uint;

const uint DEF_SIZE = 50;
const uint EXPAND_RATE = 2;
const uint RESERVE_EXP_LEN = 50;

struct VIterator {
    Vector* vector;
    uint idx;
};

struct Vector {
    void** table;
    uint size;
    uint element_count;
    uint last_elem_idx;
};

static int Vector_expand(Vector* this){
    void** new_table = calloc(this->size * 2, sizeof(void*));
    if(!new_table)
        return -1;
    memcpy(new_table, this->table, this->size);
    free(this->table);
    this->table = new_table;
    this->size *= 2;
    return 0;
}

static int Vector_aggressive_expand(Vector* this, uint index){
    uint new_size = index + RESERVE_EXP_LEN;
    void** new_table = calloc(new_size, sizeof(void*));
    if(!new_table)
        return -1;
    memcpy(new_table, this->table, this->size);
    free(this->table);
    this->size = new_size;
    return 0;
}

Vector* Vector_new(void){
    return Vector_new_with_size(DEF_SIZE);
}

Vector* Vector_new_reserve(unsigned int length_to_reserve){
    Vector* this = malloc(sizeof(Vector));
    if(!this)
        return NULL;
    this->size = length_to_reserve + RESERVE_EXP_LEN;
    this->element_count = 0;
    this->table = calloc(this->size, sizeof(void*));
    return this;
} 

void Vector_destroy(Vector* this){
    free(this->table);
    frew(this);
}

void Vector_clear(Vector* this){
    for(uint idx=0; idx<this->size; idx++)
        this->table[idx] = NULL;
    this->element_count = 0;
}



void** Vector_data(Vector* this){
    return this->table;
}

int Vector_pushback(Vector* this, const void* data){
    if(this->last_elem_idx >= this->size)
        if(Vector_expand(this) == -1)
            return -1;
    
}

int Vector_set(Vector* this, const void* data, uint index){
    if(index > this->size)
        if(Vector_aggressive_expand(this, index) == -1)
            return -1;
    this->table[index] = data;
    if(index > this->last_elem_idx)
        this->last_elem_idx = index;
    return 0;
}

int Vector_insert(Vector* this, const void* data, unsigned int index){
    
}

void* Vector_get(Vector* this, uint index){
    if(index >= this->size)
        return NULL;
    return this->table[index];
}

void* Vector_front(Vector* this){
    return this->table[0];
}

void* Vector_back(Vector* this){
    return this->table[this->push_idx - 1];
}

void* Vector_remove(Vector* this, unsigned int index){

}

VIterator* VIterator_new(Vector* vector){
    VIterator* this = malloc(sizeof(VIterator));
    this->vector;
    this->idx = 0;
    return this;
}

void VIterator_destroy(VIterator* this){
    free(this);
}

void* VIterator_peak(VIterator* this){
    return this->vector->table[this->idx];
}

void* VIterator_next(VIterator* this){
    void* ret_val = this->vector->table[this->idx];
    if(this->idx >= this->vector->push_idx){
        return NULL;
    }
    this->idx++;
    return ret_val;
}
