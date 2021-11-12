#include "Random.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>

typedef unsigned int uint;
int initialized = 0;

static inline void check_init(){
    if(initialized)
        return;
    initialized = 1;
    srand(time(NULL));
}

// include from & to
int random_range(int from, int to){
    assert(from < to);
    check_init();

    int random = rand();
    random %= to - from + 1;
    random += from;
    return random;
}

char* random_string(uint length){
    if(length == 0)
        return NULL;
    int* str = malloc(length);
    if(!str)
        return NULL;
    check_init();
    
    uint x = length / sizeof(uint);
    uint overflow = length - (x * sizeof(uint));
    int random;
    for(uint i=0; i<x; i++){
        random = rand();
        str[i] = random;
    }
    if(overflow != 0){
        random = rand();
        const char* byte_ptr = (const char*)&random;
        uint idx = x * sizeof(uint);
        for(uint i=0; i<overflow; i++)
            ((char*)str)[idx++] = *byte_ptr++;
    }
    return (char*)str;
}

char* random_string_char_range(unsigned int length, char from, char to){
    assert(from < to);
    if(length == 0)
        return NULL;
    char* str = malloc(length);
    if(!str)
        return NULL;
    check_init();

    uint x = length / sizeof(int);
    uint overflow = length - (x * sizeof(int));
    int random;
    const char* byte_ptr;
    uint byte_idx = 0;
    for(uint i=0; i<x; i++){
        random = rand();
        byte_ptr = (const char*)&random;
        for(uint j=0; j<sizeof(int); j++){
            unsigned char random_char = *byte_ptr++;
            random_char %= (to - from) + 1;
            random_char += from;
            str[byte_idx++] = random_char;
        }
            
    }
    if(overflow != 0){
        random = rand();
        byte_ptr = (const char*)&random;
        for(uint i=0; i<overflow; i++){
            unsigned char random_char = *byte_ptr++;
            random_char %= (to - from) + 1;
            random_char += from;
            str[byte_idx++] = random_char;
        }
    }
    return str;
}