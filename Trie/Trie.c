#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Trie.h"
#include <assert.h>

const uint32_t DEF_CHILDREN_COUNT = 4;

typedef union TrieContent {
    Vector* children;
    const char* str;
} TrieContent;

struct Trie {
    uint32_t weight;
    char c;
    bool is_leaf;
    TrieContent content;
};

Trie* Trie_new(void) {
    Trie* this = malloc(sizeof(struct Trie));
    if (this == NULL)
        return NULL;

    this->is_leaf = true;
    this->c = 0;
    this->content.str = NULL;
    this->weight = 0;
    return this;
}

void Trie_destroy(Trie* this) {
        if (this->is_leaf) {
        free((void*)this->content.str);
        free(this);
    }
    else {
        Trie* child;
        V_for(this->content.children, child)
            Trie_destroy(child);
        Vector_destroy(this->content.children);
        free(this);
    }
}

// status: -3 if they are the same
// status: -2 if reached str2 end
// status: -1 if reached str1 end
static int32_t first_diff_idx(const char* str1, const char* str2, int* status) {
    if (!str1) {
        *status = -1;
        return 0;
    }
    int32_t idx = 0;
    while (*str1 == *str2) {
        if (*str1 == 0) {
            *status = -3;
            return idx;
        }
        str1++;
        str2++;
        idx++;
    }
    if (*str1 == 0)
        *status = -1;
    else if (*str2 == 0)
        *status = -2;
    else
        *status = 0;
    return idx;
}

static Trie* _Trie_new_leaf(char c, const char* follow_up) {
    Trie* new_leaf = malloc(sizeof(struct Trie));
    if (new_leaf == NULL)
        return NULL;

    new_leaf->is_leaf = true;
    new_leaf->c = c;
    new_leaf->content.str = follow_up;
    new_leaf->weight = 1;
    return new_leaf;
}

static void _Trie_add_child(Trie* node, Trie* child) {
    if (node->is_leaf) {
        node->is_leaf = false;
        node->content.children = Vector_new_init_size(DEF_CHILDREN_COUNT);
    }
    Vector_pushback(node->content.children, child);
}

static void _Trie_split_leaf(Trie* node, const char* follow_up) {
    if (follow_up == NULL)
        return;
    int32_t status;
    const uint32_t init_node_weight = node->weight;
    const char* init_node_str = node->content.str;
    const int32_t diff_idx = first_diff_idx(init_node_str, follow_up, &status);
    if (status == -3) {
        node->weight++;
        return;
    }
    node->content.str = NULL;
    node->weight = 0;

    for (int32_t idx = 0; idx < diff_idx; idx++) {
        char c = init_node_str[idx];
        Trie* new_node = _Trie_new_leaf(c, NULL);
        new_node->weight = 0;
        _Trie_add_child(node, new_node);
        node = new_node;
    }

    if (status == -1) {
        Trie* new_node = follow_up[diff_idx + 1] != 0 ?
            _Trie_new_leaf(follow_up[diff_idx], strdup(follow_up + diff_idx + 1)) :
            _Trie_new_leaf(follow_up[diff_idx], NULL);
        new_node->weight = 1;
        node->weight = init_node_weight;
        _Trie_add_child(node, new_node);
    }
    else if (status == -2) {
        Trie* new_node = init_node_str[diff_idx + 1] != 0 ?
            _Trie_new_leaf(init_node_str[diff_idx], strdup(init_node_str + diff_idx + 1)) :
            _Trie_new_leaf(init_node_str[diff_idx], NULL);
        new_node->weight = init_node_weight;
        node->weight = 1;
        _Trie_add_child(node, new_node);
    }
    else {
        Trie* new_node = follow_up[diff_idx + 1] != 0 ?
            _Trie_new_leaf(follow_up[diff_idx], strdup(follow_up + diff_idx + 1)) :
            _Trie_new_leaf(follow_up[diff_idx], NULL);
        new_node->weight = 1;
        _Trie_add_child(node, new_node);

        new_node = init_node_str[diff_idx + 1] != 0 ?
            _Trie_new_leaf(init_node_str[diff_idx], strdup(init_node_str + diff_idx + 1)) :
            _Trie_new_leaf(init_node_str[diff_idx], NULL);
        new_node->weight = init_node_weight;
        _Trie_add_child(node, new_node);
    }
    free((void*)init_node_str);
}

void Trie_insert(Trie* this, const char* c_str) {
    char c;
    while ((c = *c_str) != 0) {
        if (this->is_leaf)
            break;
        else {
            Trie* child;
            bool found_path = false;
            VIterator* it = VIterator_new(this->content.children);
            while ((child = VIterator_next(it)) != NULL) {
                if (child->c == c) {
                    found_path = true;
                    this = child;
                    break;
                }
            }
            VIterator_destroy(it);
            if (!found_path)
                break;
        }
        c_str++;
    }

    if (c == 0)
        this->weight++;
    else if (this->is_leaf)
        _Trie_split_leaf(this, c_str);
    else 
        _Trie_add_child(this, _Trie_new_leaf(c, *(c_str + 1) != 0 ?
            strdup(c_str + 1) : NULL ));
}

void _Trie_print_rec(Trie* node, FILE* fd, uint32_t depth) {
    for (uint32_t i = 0; i < depth; i++)
        printf("\t");
    fprintf(fd, "Depth: %u, weight: %u, value: %c", depth, node->weight, node->c);
    if (node->is_leaf) {
        if (node->content.str != NULL)
            printf("|%s\n", node->content.str);
        else
            printf("\n");
    }
    else {
        printf("\n");
        Trie* child;
        V_for(node->content.children, child)
            _Trie_print_rec(child, fd, depth + 1);
    }
}

void Trie_print(Trie* this, FILE* fd) {
    _Trie_print_rec(this, fd, 0);
}

static uint32_t _Trie_find_best_ending_rec(Trie* node, uint32_t best_score, char* buffer, uint32_t b_len) {
    if (b_len <= 1)
        return best_score;

    if (node->is_leaf) {
        if (node->weight <= best_score)
            return best_score;
        *buffer++ = node->c;
        if (node->content.str == NULL) {
            *buffer = 0;
            return node->weight;
        }
        else if (b_len > strlen(node->content.str) + 1) {
            strcpy(buffer, node->content.str);
            return node->weight;
        }
        return best_score;
    }
    else {
        Trie* child;
        *buffer++ = node->c;
        b_len--;
        V_for(node->content.children, child)
            best_score = _Trie_find_best_ending_rec(child, best_score, buffer, b_len);
    
        if (node->weight > best_score) {
            *buffer = 0;
            return node->weight;    
        }
        else
            return best_score;
    }
    assert(0);
    return 0;
}

static uint32_t _Trie_find_best_ending(Trie* node, uint32_t best_score, char* buffer, uint32_t b_len) {
    if (node->is_leaf) {
        if (node->content.str == NULL) {
            *buffer = 0;
            return node->weight;
        }
        else if (b_len > strlen(node->content.str) + 1) {
            strcpy(buffer, node->content.str);
            return node->weight;
        }
        return 0;
    }
    
    Trie* child;
    V_for(node->content.children, child)
        best_score = _Trie_find_best_ending_rec(child, best_score, buffer, b_len);
    return best_score;
}

uint32_t Trie_search(Trie* this, const char* prefix, char* buffer, uint32_t b_len) {
    char c;
    while ((c = *prefix++) != 0) {
        if (this->is_leaf) {
            // if (*prefix == 0) {
            //     if (this->content.str != NULL) 
            //         return -1;
            // }
            // else {
            //     if (this->content.str == NULL)
            //         return -1;
            //     if (strcmp(this->content.str, prefix) != 0)
            //         return -1;
            // }
            return 0;
        }
        else {
            Trie* child;
            bool found_path = false;
            VIterator* it = VIterator_new(this->content.children);
            while ((child = VIterator_next(it)) != NULL) {
                if (child->c == c) {
                    found_path = true;
                    this = child;
                    break;
                }
            }
            VIterator_destroy(it);
            if (!found_path)
                return 0;
        }
    }
    return _Trie_find_best_ending(this, 0, buffer, b_len);
}