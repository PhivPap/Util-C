#ifndef _MY_VECTOR_
#define _MY_VECTOR_
#include <inttypes.h>

/* Opaque types */
typedef struct Vector Vector;
typedef struct VIterator VIterator;

/* Vector functions */
Vector* Vector_new(void);
Vector* Vector_new_init_size(uint32_t size);
void Vector_destroy(Vector* this);
void Vector_clear(Vector* this);
int Vector_pushback(Vector* this, const void* data);
void* Vector_get(Vector* this, uint32_t index);
int Vector_set(Vector* this, const void* data, uint32_t index);
void* Vector_front(Vector* this);
void* Vector_back(Vector* this);
void Vector_map(Vector* this, void (*func)(void *));

/* VIterator functions */
VIterator* VIterator_new(Vector* vector);
void VIterator_destroy(VIterator* this);
void* VIterator_peak(VIterator* this);
void* VIterator_next(VIterator* this);




#endif