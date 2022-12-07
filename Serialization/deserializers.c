#include <stdio.h>

void string_deserializer(FILE* fp, void** str_ref) {
    char* str = NULL;
    size_t len = 0;
    getdelim(&str, &len, '\0', fp);
    *str_ref = str;
}

void byte_deserializer(FILE* fp, void** byte_ref) {
    fread(*byte_ref, 1, 1, fp);
}

void byte4_deserializer(FILE* fp, void** word_ref) {
    fread(*word_ref, 4, 1, fp);
}

void byte8_deserializer(FILE* fp, void** dword_ref) {
    fread(*dword_ref, 8, 1, fp);
}