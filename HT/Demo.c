#include "HashTable.h"
#include <stdio.h>

void map_func_triple(void* ht_elem){
    *(int*)ht_elem *= 3;
}


int main(int argc, const char** argv){
    int a = 2, b = 3, c = 10, d = 230, e = 1000;
    HashTable* ht = HashTable_new_with_size(20);

    HashTable_insert(ht, "a", &a);
    HashTable_insert(ht, "b", &b);
    HashTable_insert(ht, "c", &c);
    HashTable_insert(ht, "d", &d);
    HashTable_insert(ht, "e", &e);

    HTIterator* iter = HTIterator_new(ht);
    void* data;
    while(data = HTIterator_next(iter))
        printf("%d\n", *(int*)data);
    HashTable_map(ht, map_func_triple);
    HTIterator_reset(iter);
    while(data = HTIterator_next(iter))
        printf("%d\n", *(int*)data);
    HTIterator_destroy(iter);
    
    HashTable_destroy(ht);
    return 0;
}