#include "../Vector/Vector.h"

/* Opaque Types */
typedef struct Trie Trie;


/* Trie methods */
Trie* Trie_new(void);
void Trie_destroy(Trie* this);
void Trie_insert(Trie* this, const char* c_str);
uint32_t Trie_search(Trie* this, const char* prefix, char* buffer, uint32_t b_len);
void Trie_print(Trie* this, FILE* fd);