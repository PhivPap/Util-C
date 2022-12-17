#include "List.h"
#include <stdlib.h>

typedef unsigned int uint;

struct ListNode {
    void* data;
    ListNode* next;
};

struct List {
    ListNode* head;
    ListNode* tail;
    uint length;
};


List* List_new(void){
    List* this = malloc(sizeof(List));
    if(!this)
        return NULL;
    this->head = NULL;
    this->tail = NULL;
    this->length = 0;
    return this;
}

void List_destroy(List* this){
    List_clear(this);
    free(this);
}

// void List_destroy_free(List* this){
//     List_clear_free(this);
//     free(this);
// }

uint List_length(List* this){
    return this->length;
}

void List_clear(List* this){
    ListNode* prev = this->head;
    if(prev == NULL)
        return;
    ListNode* node = prev->next;
    while(node != NULL){
        free(prev);
        prev = node;
        node = node->next;
    }
    free(prev);
    this->length = 0;
    this->head = NULL;
    this->tail = NULL;
}

int List_push_front(List* this, const void* data){
    ListNode* node = malloc(sizeof(ListNode));
    if(!node)
        return -1;
    node->data = (void*)data;
    node->next = this->head;
    this->head = node;
    if(this->tail == NULL)
        this->tail = node;
    this->length++;
    return 0;
}

int List_append(List* this, const void* data){
    ListNode* node = malloc(sizeof(ListNode));
    if(!node)
        return -1;
    node->data = (void*)data;
    node->next = NULL;
    if(this->length == 0){
        this->head = node;
        this->tail = node;
    }
    else {
        this->tail->next = node;
        this->tail = node;
    }
    this->length++;
    return 0;
}

int List_insert(List* this, const void* data, uint index){
    if(index == 0)
        return List_push_front(this, data);
    if(index >= this->length)
        return List_append(this, data);

    ListNode* new = malloc(sizeof(ListNode));
    if(!new)
        return -1;
    new->data = (void*)data;
    ListNode* prev;
    ListNode* node = this->head;
    uint i = 0;
    while(i++ < index){
        prev = node;
        node = node->next;
    }
    prev->next = new;
    new->next = node;
    this->length++;
    return 0;
}

void* List_get(List* this, unsigned int index){
    if(index >= this->length)
        return NULL;
    if(index + 1 == this->length)
        return this->tail->data;
    ListNode* node = this->head;
    unsigned int i = 0;
    while(i++ < index)
        node = node->next;
    return node->data;
}

void* List_remove(List* this, unsigned int index){
    void* ret_val;
    if(index >= this->length)
        return NULL;
        
    ListNode* prev = this->head;
    ListNode* node = prev->next;
    if(index == 0){
        this->head = this->head->next;
        if(this->length == 1)
            this->tail = NULL;
        ret_val = prev->data;
        free(prev);
        this->length--;
        return ret_val;
    }
    uint i = 1;
    while(i++ < index){
        prev = node;
        node = node->next;
    }
    prev->next = node->next;
    if(index + 1 == this->length)
        this->tail = prev;
    ret_val = node->data;
    free(node);
    this->length--;
    return ret_val;
}

void List_map(List* this, void (*func)(void* )){
    ListIterator iter = ListIterator_new(this);
    void* list_item;
    while((list_item = ListIterator_next(&iter)) != NULL)
        func(list_item);
}

ListIterator ListIterator_new(List* list){
    return (ListIterator) {
        .list = list,
        .current_node = list->head
    };
}

int ListIterator_has_next(ListIterator* this){
    return (this->current_node) && (this->current_node->next);
}

void* ListIterator_next(ListIterator* this){
    if(this->current_node == NULL)
        return NULL;
    void* ret_val = this->current_node->data;
    this->current_node = this->current_node->next;
    return ret_val;
}

void* ListIterator_peak(ListIterator* this){
    if(this->current_node == NULL)
        return NULL;
    return this->current_node->data;
}

void ListIterator_reset(ListIterator* this){
    this->current_node = this->list->head;
}