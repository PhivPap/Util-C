#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Vector.h"

typedef unsigned int uint;

const uint DEF_SIZE = 50;
const uint EXPAND_RATE = 2;
const uint RESERVE_EXP_LEN = 50;

struct VIterator {
    Vector* vector;
    uint index;
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

// starts searching from max to 0
static void Vector_set_last_elem_idx(Vector* this, uint max){
    for(uint idx=max; idx>0; idx--){
        if(this->table[idx]){
            this->last_elem_idx = idx;
            return;
        }
    }
    if(this->table[0])
        this->last_elem_idx = 0;
    else
        this->last_elem_idx = -1;
}

Vector* Vector_new(void){
    return Vector_new_init_size(DEF_SIZE);
}

Vector* Vector_new_init_size(unsigned int size){
    Vector* this = malloc(sizeof(Vector));
    if(!this)
        return NULL;
    this->size = size;
    this->element_count = 0;
    this->last_elem_idx = -1;
    this->table = calloc(size, sizeof(void*));
    return this;
} 

void Vector_destroy(Vector* this){
    free(this->table);
    free(this);
}

void Vector_clear(Vector* this){
    for(uint idx=0; idx<this->size; idx++)
        this->table[idx] = NULL;
    this->element_count = 0;
    this->last_elem_idx = -1;
}

void** Vector_data(Vector* this){
    return this->table;
}

int Vector_pushback(Vector* this, const void* data){
    if(!data)
        return -2;
    uint new_elem_idx = this->last_elem_idx + 1;
    if(new_elem_idx >= this->size)
        if(Vector_expand(this) == -1)
            return -1;
    this->table[new_elem_idx] = (void*)data;
    this->element_count++;
    this->last_elem_idx = new_elem_idx;
    return 0;
}

int Vector_set(Vector* this, const void* data, uint index){
    if(index > this->size)
        if(Vector_aggressive_expand(this, index) == -1)
            return -1;
    if(this->table[index]){ // if table[index] is occupied
        if(!data){ // if data is NULL remove the element at table[index].
            this->table[index] = NULL;
            this->element_count--;
            if(index == this->last_elem_idx){ // if removing last element.
                if(index == 0)
                    this->last_elem_idx = -1;
                else
                    Vector_set_last_elem_idx(this, index - 1);
            }      
        }
        else
            this->table[index] = (void*)data;
    }
    else {
        if(data){
            this->table[index] = (void*)data;
            this->element_count++;
            if(index > this->last_elem_idx)
                this->last_elem_idx = index;
        }
    }
    return 0;
}

int Vector_insert(Vector* this, const void* data, unsigned int index){
    assert(0);
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
    if(this->last_elem_idx == -1)
        return NULL;
    return this->table[this->last_elem_idx];
}

void* Vector_remove(Vector* this, unsigned int index){
    assert(0);
}

void Vector_map(Vector* this, void (*func)(void *)){
    VIterator* iter = VIterator_new(this);
    void* vec_elem;
    while(vec_elem = VIterator_next(iter))
        func(vec_elem);
    VIterator_destroy(iter);
}

VIterator* VIterator_new(Vector* vector){
    VIterator* this = malloc(sizeof(VIterator));
    this->vector = vector;
    this->index = 0;
    return this;
}

void VIterator_destroy(VIterator* this){
    free(this);
}

void* VIterator_peak(VIterator* this){
    if(this->vector->element_count == 0)
        return NULL;
    while(this->index <= this->vector->last_elem_idx){
        if(this->vector->table[this->index])
            return this->vector->table[this->index];
        this->index++;
    }
    return NULL;
}

void* VIterator_next(VIterator* this){
    if(this->vector->element_count == 0)
        return NULL;
    while(this->index <= this->vector->last_elem_idx){
        if(this->vector->table[this->index])
            return this->vector->table[this->index++];
        this->index++;
    }
    return NULL;
}

void VIterator_reset(VIterator* this){
    this->index = 0;
}