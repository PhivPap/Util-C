#include "HashSet.h"
#include <stdio.h>

int main(int argc, const char* argv[]) {
    double *data;
    double a = 0.1, b = 123, c = -0.000543, d = -256;
    HashSet* hset = HashSet_new();

    HashSet_insert(hset, &a);
    HashSet_insert(hset, &b);
    HashSet_insert(hset, &c);
    HashSet_insert(hset, &d);

    HSIterator iter = HSIterator_new(hset);
    while ((data = HSIterator_next(&iter)) != NULL)
        *data *= 0.5;

    HS_for(hset, data)
        printf("=> %f <=\n", *data);

    HashSet_destroy(hset);
    return 0;
}