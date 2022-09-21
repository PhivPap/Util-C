#ifndef _MY_HASH_TABLE_
#define _MY_HASH_TABLE_
#include <inttypes.h>


/* Types */
typedef struct HTPair {
    const char* key;
    const void* value;
} HTPair;

/* Opaque types */
typedef struct HashTable HashTable;
typedef struct HTIterator HTIterator;
typedef struct HTPairIterator HTPairIterator;
typedef struct HTKeyIterator HTKeyIterator;

/* HashTable methods */
HashTable* HashTable_new(void);
HashTable* HashTable_new_init_size(uint32_t init_size);
HashTable* HashTable_set_hash_function(HashTable* this, uint32_t (*new_hash_function)(const char* key, uint32_t table_size));
void HashTable_destroy(HashTable* this);
void HashTable_clear(HashTable* this);
uint32_t HashTable_capacity(HashTable* this);
uint32_t HashTable_element_count(HashTable* this);
int32_t HashTable_contains(HashTable* this, const char* key);
int32_t HashTable_insert(HashTable* this, const char* key, const void* value);
void* HashTable_get(HashTable* this, const char* key);
void* HashTable_remove(HashTable* this, const char* key);
int32_t HashTable_set_max_load_factor(HashTable* this, double max_load_factor);
double HashTable_get_max_load_factor(HashTable* this);
double HashTable_get_current_load_factor(HashTable* this);
void HashTable_map(HashTable* this, void (*func)(void* ));

/* HTIterator methods */
HTIterator* HTIterator_new(HashTable* hashtable);
void HTIterator_destroy(HTIterator* this);
void* HTIterator_peak(HTIterator* this);
void* HTIterator_next(HTIterator* this);
void HTIterator_reset(HTIterator* this);

/* HTKeyIterator methods */
HTKeyIterator* HTKeyIterator_new(HashTable* hashtable);
void HTKeyIterator_destroy(HTKeyIterator* this);
const char* HTKeyIterator_peak(HTKeyIterator* this);
const char* HTKeyIterator_next(HTKeyIterator* this);
void HTKeyIterator_reset(HTKeyIterator* this);

/* HTPairIterator methods */
HTPairIterator* HTPairIterator_new(HashTable* hashtable);
void HTPairIterator_destroy(HTPairIterator* this);
HTPair* HTPairIterator_peak(HTPairIterator* this);
HTPair* HTPairIterator_next(HTPairIterator* this);
void HTPairIterator_reset(HTPairIterator* this);



#endif
