#include <stdio.h>
#include "Trie.h"


void fill_trie(Trie* trie) {
    Trie_insert(trie, "barbed");
    Trie_insert(trie, "barbara");
    Trie_insert(trie, "strange");
    Trie_insert(trie, "stranger");
    Trie_insert(trie, "strangle");
    Trie_insert(trie, "strap");
    Trie_insert(trie, "strap");
    Trie_insert(trie, "string");
    Trie_insert(trie, "string");
    Trie_insert(trie, "string");
    Trie_insert(trie, "strong");
    Trie_insert(trie, "stronger");
    Trie_insert(trie, "stronger");
    Trie_insert(trie, "straw");
    Trie_insert(trie, "straw");
    Trie_insert(trie, "strawberry");
}

void print_best_match(Trie* trie, const char* prefix) {
    char buffer[20];
    uint32_t score = Trie_search(trie, prefix, buffer, sizeof(buffer));
    if (score > 0)
        printf("For prefix '%s' found '%s%s' with score %d\n", prefix, prefix, buffer, score);
    else
        printf("No match found for prefix '%s'\n", prefix);
}

int main(int argc, const char* argv[]) {
    Trie* trie = Trie_new();

    fill_trie(trie);
    // Trie_print(trie, stdout);

    print_best_match(trie, "str");
    print_best_match(trie, "stra");
    print_best_match(trie, "stri");

    Trie_destroy(trie);
    return 0;
}