#include <stdio.h>
#include <stdlib.h>

void* string_deserializer(FILE* fp) {
    char* str = NULL;
    size_t len = 0;
    getdelim(&str, &len, '\0', fp);
    return str;
}

void* byte_deserializer(FILE* fp) {
    void* byte = malloc(1);
    fread(byte, 1, 1, fp);
    return byte;
}

void* word_deserializer(FILE* fp) {
    void* word = malloc(4);
    fread(word, 4, 1, fp);
    return word;
}

void* dword_deserializer(FILE* fp) {
    void* dword = malloc(8);
    fread(dword, 8, 1, fp);
    return dword;
}