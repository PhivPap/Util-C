#include <stdlib.h>
#include <stdio.h>
#include "Vector.h"

void println(void* str) {
    puts((const char*)str);
}

void print_vec_str(Vector* vec) {
    Vector_apply(vec, println);
}

void part1(Vector* vec) {
    Vector_set(vec, 1, "<missing entry>");
    
    puts("print vec:");
    print_vec_str(vec);

    puts("print vec front:");
    puts(Vector_front(vec));

    puts("print vec back:");
    puts(Vector_back(vec));
}

void part2(Vector* vec) {
    printf("vec size: %lu\n", Vector_size(vec));
    printf("vec capacity: %lu\n", Vector_capacity(vec));

    Vector_clear(vec);
    puts("vec cleared");

    printf("vec size: %lu\n", Vector_size(vec));
    printf("vec capacity: %lu\n", Vector_capacity(vec));
}

// even or odd?
void part3(Vector* vec) {
    Vector_pushback(vec, "!!!");
    Vector_pushback(vec, "@@@");
    Vector_pushback(vec, "###");
    Vector_pushback(vec, "$$$");
    Vector_pushback(vec, "^^^");

    const bool print_even = rand() % 2;
    print_even ? puts("print even") : puts("print odd");

    const char* str;
    bool even = true;
    V_for(vec, str) {
        if (even == print_even)
            printf("-> %s\n", str);
        even = !even;
    }
}

int main() {
    Vector* vec = Vector_new();
    Vector_pushback(vec, "Lorem ipsum dolor");
    Vector_pushback(vec, "");
    Vector_pushback(vec, "sit amet, consectetur");

    puts("-- Part 1 ------------");
    part1(vec);

    puts("-- Part 2 ------------");
    part2(vec);

    puts("-- Part 3 ------------");
    part3(vec);
    
    return 0;
}