#ifndef _MY_HASH_TABLE_
#define _MY_HASH_TABLE_
#include <inttypes.h>

#define MERGE_(a,b)  a##b

#define LABEL0_(a) MERGE_(_uniq0_, a)
#define UNIQUE_NAME0 LABEL0_(__LINE__)

#define LABEL1_(a) MERGE_(_uniq1_, a)
#define UNIQUE_NAME1 LABEL1_(__LINE__)


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
void HashTable_set_hash_function(HashTable* this, uint32_t (*new_hash_function)(const char* key, uint32_t table_size));
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
void* HTIterator_destroy(HTIterator* this);
void* HTIterator_peak(HTIterator* this);
void* HTIterator_next(HTIterator* this);
void HTIterator_reset(HTIterator* this);
#define HT_for(ht, val) \
for ( \
    HTIterator* UNIQUE_NAME0 = HTIterator_new(ht); \
    (val = HTIterator_next(UNIQUE_NAME0)) != NULL || HTIterator_destroy(UNIQUE_NAME0); \
)

/* HTKeyIterator methods */
HTKeyIterator* HTKeyIterator_new(HashTable* hashtable);
void HTKeyIterator_destroy(HTKeyIterator* this);
const char* HTKeyIterator_peak(HTKeyIterator* this);
const char* HTKeyIterator_next(HTKeyIterator* this);
void HTKeyIterator_reset(HTKeyIterator* this);
#define HTKey_for(ht, key) \
for ( \
    HTIterator* UNIQUE_NAME0 = HTKeyIterator_new(ht); \
    (key = HTKeyIterator_next(UNIQUE_NAME0)) != NULL || HTKeyIterator_destroy(UNIQUE_NAME0); \
)

/* HTPairIterator methods */
HTPairIterator* HTPairIterator_new(HashTable* hashtable);
void* HTPairIterator_destroy(HTPairIterator* this);
HTPair* HTPairIterator_peak(HTPairIterator* this);
HTPair* HTPairIterator_next(HTPairIterator* this);
void HTPairIterator_reset(HTPairIterator* this);
#define HTPair_for(ht, key, val) \
HTPair* UNIQUE_NAME0; \
for ( \
    HTPairIterator* UNIQUE_NAME1 = HTPairIterator_new(ht); \
    ((UNIQUE_NAME0 = HTPairIterator_next(UNIQUE_NAME1)) != NULL && (key = UNIQUE_NAME0->key) && (val = (typeof(val))UNIQUE_NAME0->value)) || HTPairIterator_destroy(UNIQUE_NAME1); \
)



#endif
