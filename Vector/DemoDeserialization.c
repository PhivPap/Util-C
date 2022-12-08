#include "Vector.h"
#include "../Serialization/deserializers.h"
#include <stdlib.h>

const char* vec_bin_path = "vec.bin";

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