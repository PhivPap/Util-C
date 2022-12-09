#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"

    
const char* LIBS[] = { "HashSet", "HashTable", "Json", "List", "Random", "StopWatch", "String", "Vector" };
const uint32_t LIB_SCORES[] = { 3, 9, 10, 4, 1, 8, 2, 7 };
const uint32_t LIBS_SIZE = sizeof(LIBS) / sizeof(LIBS[0]);  
    




void fill_ht(HashTable* ht, const char* keys[], const uint32_t* values, uint32_t len) {
    for (uint32_t i = 0; i < len; i++)
        HashTable_insert(ht, keys[i], values + i);
}

void print_ht(HashTable* ht) {
    const char* key;
    int32_t* val;
    int32_t avg_val = 0;
    HTPair_for(ht, key, val) {
        avg_val += *val;
        printf("%s: %d\n", key, *val);
    }
    printf("Average score: %.2f\n", (double)avg_val / HashTable_size(ht)); // unsafe div :O
}


int main(int argc, const char* argv[]) {
    HashTable* ht = HashTable_new();

    fill_ht(ht, LIBS, LIB_SCORES, LIBS_SIZE);
    print_ht(ht);

    HashTable_destroy(ht);
    return 0;
}