#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Random.h"


void main(){
    int random = random_range(10, 20);
    int random2 = random_range(10, 20);
    printf("%d - %d\n", random, random2);

    char* str = random_string(5);
    char* str2 = random_string_char_range(9, 'a', 'z');
    printf("%s %lu\n", str, strlen(str));
    printf("%s %lu\n", str2, strlen(str2));
    free(str);
    free(str2);
}