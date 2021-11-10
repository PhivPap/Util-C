#ifndef _MY_LIST_
#define _MY_LIST_

/* Opaque types */
typedef struct List List;
typedef struct ListIterator ListIterator;

/* List functions */
List* List_new(void);
void List_destroy(List* this);
void List_destroy_free(List* this);
void List_clear(List* this);
void List_clear_free(List* this);
unsigned int List_length(List* this);
void List_push_front(List* this, const void* data);
void List_append(List* this, const void* data);
void List_insert(List* this, const void* data, unsigned int index);
void* List_get(List* this, unsigned int index);
void* List_remove(List* this, unsigned int index);

/* ListIterator functions */
ListIterator* ListIterator_new(List* list);
void ListIterator_destroy(ListIterator* this);
void* ListIterator_next(ListIterator* this);
void* ListIterator_peak(ListIterator* this);
void ListIterator_reset(ListIterator* this);

#endif

/* DOC
    List:
        Implementation of a sigle linked list with O(1) push_front and append functions.
        Unchecked malloc(...).

        List_clear(...) 
            - clears the list (does not call free() on data).

        List_clear_free(...)
            - danger!
            - clears the list and frees all elements. Erroneous if elements we not allocated from heap.

        List_destroy(...*)
            - calls List_clear(...)

        List_destroy_free(...)
            - calls List_clear_free(...)


    ListIterator:
        Implementation of a read-only List Iterator..
        Unchecked malloc().

        ListIterator_peak(...) 
            - returns NULL if the iterator has reached the end of the list.
            - Erroneous to call after its List has been modified, call ListIterator_reset(...).

        ListIterator_next(...)
            - returns NULL if the iterator has reached the end of the list.
            - returns current element of the list. After the call, the iterator is pointing to the next element.
            - Erroneous to call after its List has been modified, call ListIterator_reset(...).
 */