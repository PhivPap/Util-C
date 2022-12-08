#include <stdio.h>

void* string_deserializer(FILE* fp);
void* byte_deserializer(FILE* fp);
void* word_deserializer(FILE* fp);  // 4 bytes
void* dword_deserializer(FILE* fp); // 8 bytes