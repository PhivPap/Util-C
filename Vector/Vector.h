#ifndef _MY_VECTOR_
#define _MY_VECTOR_

/* Opaque types */
typedef struct Vector Vector;
typedef struct VIterator VIterator;

/* Vector functions */
Vector* Vector_new(void);
Vector* Vector_new_init_size(unsigned int size);
void Vector_destroy(Vector* this);
void Vector_clear(Vector* this);
int Vector_pushback(Vector* this, const void* data);
int Vector_insert(Vector* this, const void* data, unsigned int index);
void* Vector_get(Vector* this, unsigned int index);
int Vector_set(Vector* this, const void* data, unsigned int index);
void* Vector_front(Vector* this);
void* Vector_back(Vector* this);
void* Vector_remove(Vector* this, unsigned int index);

/* VIterator functions */
VIterator* VIterator_new(Vector* vector);
void VIterator_destroy(VIterator* this);
void* VIterator_peak(VIterator* this);
void* VIterator_next(VIterator* this);




#endif