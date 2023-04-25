#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "Vector.h"

const uint64_t DEF_SIZE = 50;
const uint64_t EXPAND_RATE = 2;
const uint64_t RESERVE_EXP_LEN = 50;

struct Vector {
    void** table;
    uint64_t capacity;
    uint64_t element_count;
};

static int Vector_expand(Vector* this) {
    void** new_table = calloc(this->capacity * 2, sizeof(void*));
    if(!new_table)
        return -1;
    memcpy(new_table, this->table, this->capacity * sizeof(void*));
    free(this->table);
    this->table = new_table;
    this->capacity *= 2;
    return 0;
}

Vector* Vector_new(void) {
    return Vector_new_init_size(DEF_SIZE);
}

Vector* Vector_new_init_size(uint64_t size) {
    Vector* this = malloc(sizeof(Vector));
    if(!this)
        return NULL;
    this->capacity = size;
    this->element_count = 0;
    this->table = calloc(size, sizeof(void*));
    if (this->table == NULL) {
        free(this);
        return NULL;
    }
    return this;
} 

void Vector_destroy(Vector* this) {
    free(this->table);
    free(this);
}

uint64_t Vector_size(Vector* this) {
    return this->element_count;
}

uint64_t Vector_capacity(Vector* this) {
    return this->capacity;
}

void Vector_clear(Vector* this) {
    for(int32_t idx = 0; idx < this->capacity; idx++)
        this->table[idx] = NULL;
    this->element_count = 0;
}

void** Vector_data(Vector* this) {
    return this->table;
}

int Vector_pushback(Vector* this, const void* data) {
    if(!data)
        return -2;
    int32_t new_elem_idx = this->element_count;
    if(new_elem_idx >= this->capacity)
        if(Vector_expand(this) == -1)
            return -1;
    this->table[new_elem_idx] = (void*)data;
    this->element_count++;
    return 0;
}

int Vector_set(Vector* this, uint64_t idx, const void* data) {
    if (idx > this->element_count)
        return -1;
    else if (idx == this->element_count)
        return Vector_pushback(this, data);
    else if (data == NULL)
        return -2;
    this->table[idx] = (void*)data;
    return 0;
}

void* Vector_get(Vector* this, uint64_t idx) {
    if(idx >= this->element_count)
        return NULL;
    return this->table[idx];
}

void* Vector_front(Vector* this) {
    return this->table[0];
}

void* Vector_back(Vector* this) {
    if (this->element_count == 0)
        return NULL;
    return this->table[this->element_count - 1];
}

void Vector_apply(Vector* this, void (*func)(void *)) {
    void* item;
    V_for(this, item)
        func(item);
}

VIterator VIterator_new(Vector* vector) {
    return (VIterator) { .vector = vector, .idx = 0 };
}

void* VIterator_peak(VIterator* this) {
    Vector* vector = this->vector;
    if(this->idx >= vector->element_count)
        return NULL;
    return vector->table[this->idx];
}

void* VIterator_next(VIterator* this) {
    Vector* vector = this->vector;
    if(this->idx >= vector->element_count)
        return NULL;
    void* ret_val = vector->table[this->idx];
    this->idx++;
    return ret_val;
}

void VIterator_reset(VIterator* this) {
    this->idx = 0;
}

void Vector_serialize(Vector* this, FILE* fp, void (*item_serializer)(FILE* fp, void* item)) {
    void* item;
    fwrite(&(this->element_count), sizeof(this->element_count), 1, fp);
    V_for(this, item) 
        item_serializer(fp, item);
}

Vector* Vector_deserialize(FILE* fp, void* (*item_deserializer)(FILE* fp)) {
    uint64_t element_count;
    fread(&element_count, sizeof(element_count), 1, fp);
    Vector* this = Vector_new_init_size(element_count);
    if (this == NULL)
        return NULL;

    for (uint64_t i = 0; i < element_count; i++) {
        void* item = item_deserializer(fp);
        if (item != NULL)
            Vector_pushback(this, item);
    }
    return this;
}