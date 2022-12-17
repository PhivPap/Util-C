#include <stdio.h>

#include "Vector.h"

int main(int argc, const char* argv[]){
    Vector* vec = Vector_new_init_size(2);
    int a = 2, b = 3, c = 23, d = 100, e = 99999;
    Vector_pushback(vec, &a);
    Vector_pushback(vec, &b);
    Vector_pushback(vec, &c);
    Vector_pushback(vec, &d);

    Vector_set(vec, &e, 10);


    VIterator iter = VIterator_new(vec);
    int* data;
    while((data = VIterator_next(&iter)) != NULL)
        *data += 10;

    V_for(vec, data)
        printf("%d\n", *data);

    Vector_destroy(vec);

    return 0;
}