#include "Vector.h"
#include <stdlib.h>

const char* vec_bin_path = "vec.bin";

void string_deserializer(FILE* fp, void** str_ref) {
    char* str = NULL;
    size_t len = 0;
    getdelim(&str, &len, '\0', fp);
    *str_ref = str;
}

int main(int argc, const char** argv) {
    FILE* fp = fopen(vec_bin_path, "r");
    if (fp == NULL) {
        printf("> Cannot open '%s' for reading\n", vec_bin_path);
        return -1;
    }

    Vector* vec = Vector_deserialize(fp, string_deserializer);
    printf("> Deserialization complete\n");

    char* str;
    V_for(vec, str)
        puts(str);

    Vector_map(vec, free);
    Vector_destroy(vec);
    fclose(fp);
}