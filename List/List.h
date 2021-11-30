#ifndef _MY_LIST_
#define _MY_LIST_

/* Opaque types */
typedef struct List List;
typedef struct ListIterator ListIterator;

/* List methods */
List* List_new(void);
void List_destroy(List* this);
void List_clear(List* this);
unsigned int List_length(List* this);
int List_push_front(List* this, const void* data);
int List_append(List* this, const void* data);
int List_insert(List* this, const void* data, unsigned int index);
void* List_get(List* this, unsigned int index);
void* List_remove(List* this, unsigned int index);
void List_map(List* this, void (*func)(void* ));

/* ListIterator methods */
ListIterator* ListIterator_new(List* list);
void ListIterator_destroy(ListIterator* this);
int ListIterator_has_next(ListIterator* this);
void* ListIterator_next(ListIterator* this);
void* ListIterator_peak(ListIterator* this);
void ListIterator_reset(ListIterator* this);

#endif
