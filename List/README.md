# List
Implementation of a sigle linked list with O(1) push_front and append functions.

Unchecked malloc(...).

    List_clear(...):
      Clears the list (does not call free() on data).
    List_clear_free(...):
      Clears the list and frees all elements. Don't call if elements are not heap.
    List_destroy(...*):
      Clears the list and selfdestructs.
    List_destroy_free(...)
      Clears the list with  List_clear_free(...) and selfdestructs.
      
## ListIterator:
Implementation of a read-only List Iterator..

Unchecked malloc().

    ListIterator_peak(...) 
      Returns NULL if the iterator has reached the end.
      Don't call after the referring List has been modified, call ListIterator_reset(...) first.
    ListIterator_next(...)
      Returns NULL if the iterator has reached the end of the list.
      Returns current element of the list. After the call, the iterator is pointing to the next element.
      Don't call after the referring List has been modified, call ListIterator_reset(...) first.
