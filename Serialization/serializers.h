#include <stdio.h>

void string_serializer(FILE* fp, void* str);
void byte_serializer(FILE* fp, void* byte);
void word_serializer(FILE* fp, void* word);     // 4 bytes
void dword_serializer(FILE* fp, void* dword);   // 8 bytes