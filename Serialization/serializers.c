#include "serializers.h"

void string_serializer(FILE* fp, void* str) {
    fprintf(fp, "%s%c", (char*)str, '\0');
}

void byte_serializer(FILE* fp, void* byte) {
    fputc(*(char*)byte, fp);
}

void byte4_serializer(FILE* fp, void* word) {
    fwrite(word, 4, 1, fp);
}

void byte8_serializer(FILE* fp, void* dword) {
    fwrite(dword, 8, 1, fp);
}