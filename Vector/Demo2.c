#include <stdio.h>
#include "Vector.h"

const char* LIBS[] = { "HashSet", "HashTable", "Json", "List", "Random", "StopWatch", "String", "Vector" };
const uint32_t LIBS_SIZE = sizeof(LIBS) / sizeof(LIBS[0]);



void init_vec(Vector* vec, const char* values[], uint32_t len) {
    for (uint32_t i = 0; i < len; i++)
        Vector_pushback(vec, LIBS[i]);
}

void print_all_combs(Vector* vec) {
    const char* str1, * str2;
    V_for(vec, str1)
        V_for(vec, str2)
            if (str1 != str2)
                printf("%s - %s\n", str1, str2);
}

int main(int argc, const char* argv[]) {
    Vector* vec = Vector_new();
    init_vec(vec, LIBS, LIBS_SIZE);
    print_all_combs(vec);
    Vector_destroy(vec);
    return 0;
}