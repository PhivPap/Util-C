#ifndef _MY_HASH_TABLE_
#define _MY_HASH_TABLE_
#include <inttypes.h>
#include <stdio.h>

#define _MERGE_(prefix, num) prefix##num
#define _LABEL_(num) _MERGE_(_uniq_, num)
#define _UNIQUE_ID_ _LABEL_(__COUNTER__)

/* Opaque types */
typedef struct HashTable HashTable;

/* Types */
typedef struct HTPair {
    const char* key;
    const void* value;
} HTPair;

typedef struct HTIterator {
    HashTable* hashtable;
    uint32_t index;
} HTIterator;

typedef struct HTKeyIterator {
    HashTable* hashtable;
    uint32_t index;
} HTKeyIterator;

typedef struct HTPairIterator {
    HashTable* hashtable;
    uint32_t index;
    HTPair pair;
} HTPairIterator;

/* HashTable methods */
HashTable* HashTable_new(void);
HashTable* HashTable_new_init_size(uint32_t init_size);
void HashTable_set_hash_function(HashTable* this, uint32_t (*new_hash_function)(const char* key, uint32_t table_size));
void HashTable_destroy(HashTable* this);
void HashTable_clear(HashTable* this);
uint32_t HashTable_capacity(HashTable* this);
uint32_t HashTable_size(HashTable* this);
int32_t HashTable_contains(HashTable* this, const char* key);
int32_t HashTable_insert(HashTable* this, const char* key, const void* value);
void* HashTable_get(HashTable* this, const char* key);
void* HashTable_remove(HashTable* this, const char* key);
int32_t HashTable_set_max_load_factor(HashTable* this, double max_load_factor);
double HashTable_get_max_load_factor(HashTable* this);
double HashTable_get_current_load_factor(HashTable* this);
void HashTable_map(HashTable* this, void (*func)(void* ));
void HashTable_serialize(HashTable* this, FILE* fp, void (*value_serializer)(FILE* fp, void* value));
HashTable* HashTable_deserialize(FILE* fp, void* (*value_deserializer)(FILE* fp));

/* HTIterator methods + macro */
HTIterator HTIterator_new(HashTable* hashtable);
void* HTIterator_peak(HTIterator* this);
void* HTIterator_next(HTIterator* this);
void HTIterator_reset(HTIterator* this);
#define _HT_for_(_ht, _val, unique_id) \
for ( \
    HTIterator unique_id = HTIterator_new(_ht); \
    (_val = HTIterator_next(&unique_id)) != NULL; \
)
#define HT_for(ht, val) _HT_for_(ht, val, _UNIQUE_ID_)

/* HTKeyIterator methods + macro */
HTKeyIterator HTKeyIterator_new(HashTable* hashtable);
const char* HTKeyIterator_peak(HTKeyIterator* this);
const char* HTKeyIterator_next(HTKeyIterator* this);
void HTKeyIterator_reset(HTKeyIterator* this);

#define _HTKey_for_(ht, _key, unique_id0) \
for ( \
    HTKeyIterator unique_id0 = HTKeyIterator_new(ht); \
    (_key = HTKeyIterator_next(&unique_id0)) != NULL; \
)
#define HTKey_for(ht, key) _HTKey_for_(ht, key, _UNIQUE_ID_)

/* HTPairIterator methods + macro */
HTPairIterator HTPairIterator_new(HashTable* hashtable);
HTPair* HTPairIterator_peak(HTPairIterator* this);
HTPair* HTPairIterator_next(HTPairIterator* this);
void HTPairIterator_reset(HTPairIterator* this);

#define _HTPair_for_(_ht, _key, _val, unique_id0, unique_id1) \
HTPair* unique_id0; \
for ( \
    HTPairIterator unique_id1 = HTPairIterator_new(_ht); \
    (unique_id0 = HTPairIterator_next(&unique_id1)) != NULL && (_key = unique_id0->key) && (_val = (typeof(_val))unique_id0->value); \
)
#define HTPair_for(ht, key, val) _HTPair_for_(ht, key, val, _UNIQUE_ID_, _UNIQUE_ID_)

#endif


