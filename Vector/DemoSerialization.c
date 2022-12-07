#include "Vector.h"

const char* vec_bin_path = "vec.bin";

void fill_vector(Vector* vec) {
    Vector_pushback(vec, "analyst");
    Vector_pushback(vec, "margin");
    Vector_pushback(vec, "insertion");
    Vector_pushback(vec, "software");
    Vector_pushback(vec, "bake");
    Vector_pushback(vec, "soprano");
    Vector_pushback(vec, "gallery");
    Vector_pushback(vec, "parade");
    Vector_pushback(vec, "stretch");
    Vector_pushback(vec, "landlord");
}

void string_serializer(FILE* fp, void* str) {
    fprintf(fp, "%s%c", (char*)str, '\0');
}

int main(int argc, const char** argv) {
    Vector* vec = Vector_new_init_size(10);
    fill_vector(vec);

    FILE* fp = fopen("vec.bin", "w");
    if (fp == NULL) {
        printf("> Cannot open '%s' for reading\n", vec_bin_path);
        return -1;
    }

    Vector_serialize(vec, fp, string_serializer);
    printf("> Serialization complete\n");

    fclose(fp);
    Vector_destroy(vec);
}