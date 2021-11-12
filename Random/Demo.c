#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Random.h"


void main(){
    int random = random_int(10, 20);
    char random2 = random_char('a', 'g');
    double random3 = random_double(-2.0, 2.5);
    printf("%d - %c - %lf\n", random, random2, random3);

    char* str = random_string(5);
    char* str2 = random_string_char_range(9, 'a', 'z');
    printf("%s %lu\n", str, strlen(str)); // len might be less than 5 if a null terminator was generated.
    printf("%s %lu\n", str2, strlen(str2));
    free(str);
    free(str2);
}