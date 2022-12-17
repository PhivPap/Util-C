#ifndef _MY_HASH_SET_
#define _MY_HASH_SET_
#include <inttypes.h>
#include <stdbool.h>

#define _MERGE_(prefix, num) prefix##num
#define _LABEL_(num) _MERGE_(_uniq_, num)
#define _UNIQUE_ID_ _LABEL_(__COUNTER__)

/* Opaque types */
typedef struct HashSet HashSet;

/* Types */
typedef struct HSIterator {
    HashSet* set;
    uint32_t index;
} HSIterator;

/* HashSet methods */
HashSet* HashSet_new(void);
HashSet* HashSet_new_init_size(uint32_t init_size);
void HashSet_set_hash_function(HashSet* this, uint32_t (*new_hash_function)(const void* data, uint32_t table_size));
void HashSet_destroy(HashSet* this);
void HashSet_clear(HashSet* this);
uint32_t HashSet_capacity(HashSet* this);
uint32_t HashSet_element_count(HashSet* this);
int32_t HashSet_contains(HashSet* this, void* data);
int32_t HashSet_insert(HashSet* this, void* data);
void* HashSet_remove(HashSet* this, void* data);
int32_t HashSet_set_max_load_factor(HashSet* this, double max_load_factor);
double HashSet_get_max_load_factor(HashSet* this);
double HashSet_get_current_load_factor(HashSet* this);
void HashSet_map(HashSet* this, void (*func)(void* ));

/* HSIterator methods + macro */
HSIterator HSIterator_new(HashSet* HashSet);
void* HSIterator_peak(HSIterator* this);
void* HSIterator_next(HSIterator* this);
void HSIterator_reset(HSIterator* this);

#define _HS_for_(_hset, _val, unique_id) for (HSIterator unique_id = HSIterator_new(_hset); (_val = HSIterator_next(&unique_id)) != NULL; )
#define HS_for(hset, val) _HS_for_(hset, val, _UNIQUE_ID_)


#endif
