#include <stdio.h>

void string_deserializer(FILE* fp, void** str_ref);
void byte_deserializer(FILE* fp, void** byte_ref);
void byte4_deserializer(FILE* fp, void** word_ref);
void byte8_deserializer(FILE* fp, void** dword_ref);