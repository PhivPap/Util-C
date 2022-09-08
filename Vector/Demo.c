#include <stdio.h>

#include "Vector.h"




void main(){
    Vector* vec = Vector_new_init_size(2);
    int a = 2, b = 3, c = 23, d = 100, e = 99999;
    Vector_pushback(vec, &a);
    Vector_pushback(vec, &b);
    Vector_pushback(vec, &c);
    Vector_pushback(vec, &d);

    Vector_set(vec, &e, 10);


    VIterator* iter = VIterator_new(vec);
    void* data;
    while(data = VIterator_next(iter))
        printf("%d\n", *(int*)data);

    VIterator_destroy(iter);
    Vector_destroy(vec);
}