#include <stdio.h>

void string_serializer(FILE* fp, void* str);
void byte_serializer(FILE* fp, void* byte);
void byte4_serializer(FILE* fp, void* word);
void byte8_serializer(FILE* fp, void* dword);