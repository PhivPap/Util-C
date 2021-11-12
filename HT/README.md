# HashTable
Implementation of an open address hash table with linear probing.

Checked malloc(...)
  
    HashTable_new(...), HashTable_new_with_size(...):
        Both constructors return null if heap insufficient.
    HashTable_contains(...):
        Returns 1 if the key is contained in the HashTable, otherwise 0.
    HashTable_insert(...):
        Undefined if key already exists in the HashTable.
        Returns -1 if memory is insufficient.
    HashTable_get(...):
        Returns NULL if key does not exist in the HashTable.
    HashTable_remove(...):
        Returns the value correspoding to the given key or NULL if key does not exist in the HashTable.
    HashTable_set_max_load_factor(...):
        Returns 0 if value accepted and -1 otherwise.
        Table may resize only after an insertion (not on this function call).
# HTIterator
  Implementation of a read-only HashTable Iterator.
  
  Checked malloc(...).
  
    HTIterator_peak(...) 
        Returns NULL if the iterator has reached the end.
        Don't call after the referring HashTable has been modified, call HTIterator_reset(...) first.
    HTIterator_peak(...)
        Returns NULL if the iterator has reached the end.
        After the call, the iterator is pointing to the next element.
        Don't call after the referring HashTable has been modified, call HTIterator_reset(...) first.
