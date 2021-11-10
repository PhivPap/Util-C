#ifndef _MY_HASH_TABLE_
#define _MY_HASH_TABLE_

/* Opaque types */
typedef struct HashTable HashTable;
typedef struct HTIterator HTIterator;

/* HashTable functions */
HashTable* HashTable_new(void);
HashTable* HashTable_new_with_size(unsigned int init_size);
void HashTable_destroy(HashTable* this);
unsigned int HashTable_capacity(HashTable* this);
unsigned int HashTable_element_count(HashTable* this);
int HashTable_contains(HashTable* this, const char* key);
int HashTable_insert(HashTable* this, const char* key, const void* value);
void* HashTable_get(HashTable* this, const char* key);
void* HashTable_remove(HashTable* this, const char* key);
int HashTable_set_max_load_factor(HashTable* this, double max_load_factor);
double HashTable_get_max_load_factor(HashTable* this);
double HashTable_get_current_load_factor(HashTable* this);

/* HTIterator functions */
HTIterator* HTIterator_new(HashTable* hashtable);
HTIterator* HTIterator_destroy(HTIterator* this);
void* HTIterator_peak(HTIterator* this);
void* HTIterator_next(HTIterator* this);
void HTIterator_reset(HTIterator* this);

#endif

/* DOC
    HashTable:
        Implementation of open address HashTable with linear probing.
        Checked malloc(...).

        HashTable_new(...), HashTable_new_with_size(...):
            - both constructors return null if heap insufficient.

        HashTable_contains(...):
            - returns 1 if the key is contained in the HashTable, otherwise 0.

        HashTable_insert(...):
            - Erroneous if key already exists in the HashTable.
            - returns -1 if memory is insufficient.

        HashTable_get(...):
            - returns NULL if key does not exist in the HashTable.

        HashTable_remove(...):
            - returns the value correspoding to the given key or NULL if key does not exist in the HashTable.

        HashTable_set_max_load_factor(...):
            - returns 0 if value accepted and -1 otherwise.
            - table may resize only after an insertion (not on this function call).



    HTIterator:
        Implementation of a read-only HashTable Iterator.
        Checked malloc(...).

        HTIterator_peak(...) 
            - returns NULL if the iterator has reached the end of the hash table.
            - Erroneous to call after its HashTable has been modified, call HTIterator_reset(ListIterator*).

        HTIterator_peak(...)
            - returns NULL if the iterator has reached the end of the hash table.
            - After the call, the iterator is pointing to the next element.
            - Erroneous to call after its HashTable has been modified, call HTIterator_reset(ListIterator*).

*/
