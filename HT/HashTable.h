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
