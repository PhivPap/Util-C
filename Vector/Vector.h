#ifndef _MY_VECTOR_
#define _MY_VECTOR_
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#define _MERGE_(prefix, num) prefix##num
#define _LABEL_(num) _MERGE_(_uniq_, num)
#define _UNIQUE_ID_ _LABEL_(__COUNTER__)

/* Opaque types */
typedef struct Vector Vector;

/* Types */
typedef struct VIterator {
    Vector* vector;
    int32_t index;
} VIterator;

/* Vector methods */
Vector* Vector_new(void);
Vector* Vector_new_init_size(uint32_t size);
void Vector_destroy(Vector* this);
uint32_t Vector_size(Vector* this);
uint32_t Vector_capacity(Vector* this);
void Vector_clear(Vector* this);
int Vector_pushback(Vector* this, const void* data);
void* Vector_get(Vector* this, uint32_t index);
int Vector_set(Vector* this, const void* data, uint32_t index);
void* Vector_front(Vector* this);
void* Vector_back(Vector* this);
void Vector_map(Vector* this, void (*func)(void *));
void Vector_serialize(Vector* this, FILE* fp, void (*item_serializer)(FILE* fp, void* item));
Vector* Vector_deserialize(FILE* fp, void* (*item_deserializer)(FILE* fp));

/* VIterator methods + macro */
VIterator VIterator_new(Vector* vector);
void* VIterator_peak(VIterator* this);
void* VIterator_next(VIterator* this);

#define _V_for_(_vec, _val, unique_id) for (VIterator unique_id = VIterator_new(_vec); (_val = VIterator_next(&unique_id)) != NULL; )
#define V_for(vec, val) _V_for_(vec, val, _UNIQUE_ID_)


#endif