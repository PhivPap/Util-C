#ifndef _MY_HASH_TABLE_
#define _MY_HASH_TABLE_

/* Types */
typedef struct HTPair {
    const char* key;
    const void* value;
} HTPair;

/* Opaque types */
typedef struct HashTable HashTable;
typedef struct HTIterator HTIterator;
typedef struct HTPairIterator HTPairIterator;

/* HashTable methods */
HashTable* HashTable_new(void);
HashTable* HashTable_new_with_size(unsigned int init_size);
void HashTable_destroy(HashTable* this);
void HashTable_clear(HashTable* this);
unsigned int HashTable_capacity(HashTable* this);
unsigned int HashTable_element_count(HashTable* this);
int HashTable_contains(HashTable* this, const char* key);
int HashTable_insert(HashTable* this, const char* key, const void* value);
void* HashTable_get(HashTable* this, const char* key);
void* HashTable_remove(HashTable* this, const char* key);
int HashTable_set_max_load_factor(HashTable* this, double max_load_factor);
double HashTable_get_max_load_factor(HashTable* this);
double HashTable_get_current_load_factor(HashTable* this);
void HashTable_map(HashTable* this, void (*func)(void* ));

/* HTIterator methods */
HTIterator* HTIterator_new(HashTable* hashtable);
HTIterator* HTIterator_destroy(HTIterator* this);
void* HTIterator_peak(HTIterator* this);
void* HTIterator_next(HTIterator* this);
void HTIterator_reset(HTIterator* this);

/* HTPairIterator methods */
HTPairIterator* HTPairIterator_new(HashTable* hashtable);
HTPairIterator* HTPairIterator_destroy(HTPairIterator* this);
HTPair* HTPairIterator_peak(HTPairIterator* this);
HTPair* HTPairIterator_next(HTPairIterator* this);
void HTPairIterator_reset(HTPairIterator* this);



#endif
