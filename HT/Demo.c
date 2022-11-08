#include "HashTable.h"
#include <stdio.h>

void map_func_triple(void* ht_elem){
    *(int*)ht_elem *= 3;
}


int main(int argc, const char** argv){
    int a = 2, b = 3, c = 10, d = 230, e = 1000;
    HashTable* ht = HashTable_new_init_size(20);

    HashTable_insert(ht, "a", &a);
    HashTable_insert(ht, "b", &b);
    HashTable_insert(ht, "c", &c);
    HashTable_insert(ht, "d", &d);
    HashTable_insert(ht, "e", &e);

    // =============================================

    printf("========= Iterator =========\n");
    HashTable_map(ht, map_func_triple);
    HTIterator* iter = HTIterator_new(ht);
    void* data;
    const char* key;
    while(data = HTIterator_next(iter))
        printf("%d\n", *(int*)data);
    HTIterator_destroy(iter);

    // =============================================

    printf("======= Key iterator =======\n");
    HTKeyIterator* key_iter = HTKeyIterator_new(ht);
    while (key = HTKeyIterator_next(key_iter))
        printf("%s\n", key);
    HTKeyIterator_destroy(key_iter);
    
    // =============================================

    printf("======= For iterator =======\n");
    int* data_i;
    HT_for(ht, data_i)
        *data_i += 50;

    HashTable_map(ht, map_func_triple);

    HT_for(ht, data_i)
        printf("%d\n", *data_i);


    HashTable_destroy(ht);
    return 0;
}