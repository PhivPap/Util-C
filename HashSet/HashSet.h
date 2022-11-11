#ifndef _MY_HASH_SET_
#define _MY_HASH_SET_
#include <inttypes.h>
#include <stdbool.h>

#define MERGE_(a,b)  a##b
#define LABEL0_(a) MERGE_(_uniq0_, a)
#define UNIQUE_NAME LABEL0_(__LINE__)

/* Opaque types */
typedef struct HashSet HashSet;
typedef struct HSIterator HSIterator;

/* HashSet methods */
HashSet* HashSet_new(void);
HashSet* HashSet_new_init_size(uint32_t init_size);
HashSet* HashSet_set_hash_function(HashSet* this, uint32_t (*new_hash_function)(const void* data, uint32_t table_size));
void HashSet_destroy(HashSet* this);
void HashSet_clear(HashSet* this);
uint32_t HashSet_capacity(HashSet* this);
uint32_t HashSet_element_count(HashSet* this);
int32_t HashSet_contains(HashSet* this, void* data);
int32_t HashSet_insert(HashSet* this, void* data);
// void* HashSet_get(HashSet* this, const char* key);
void* HashSet_remove(HashSet* this, void* data);
int32_t HashSet_set_max_load_factor(HashSet* this, double max_load_factor);
double HashSet_get_max_load_factor(HashSet* this);
double HashSet_get_current_load_factor(HashSet* this);
void HashSet_map(HashSet* this, void (*func)(void* ));

/* HSIterator methods */
HSIterator* HSIterator_new(HashSet* HashSet);
void HSIterator_destroy(HSIterator* this);
void* HSIterator_peak(HSIterator* this);
void* HSIterator_next(HSIterator* this);
void HSIterator_reset(HSIterator* this);
bool _HSIterator_destroy(HSIterator* this); 
#define HS_for(hset, val) for (HSIterator* UNIQUE_NAME = HSIterator_new(hset); (val = HSIterator_next(UNIQUE_NAME)) != NULL || _HSIterator_destroy(UNIQUE_NAME);)



#endif
