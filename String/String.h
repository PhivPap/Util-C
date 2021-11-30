#ifndef _MY_STRING_
#define _MY_STRING_

#include "../List/List.h"

/* Opaque types */
typedef struct String String;
typedef struct StringIterator StringIterator;

/* String methods */
String* String_new();
String* String_new_reserve(unsigned int size);
String* String_new_copy(const char* c_str);
//String* String_new_combine();
String* String_clone(const String* this);
void String_destroy(String* this);
const char* String_data(const String* this);
char* String_clone_data(const String* this);
int String_append_str(String* this, const String* str);
int String_append_c_str(String* this, const char* c_str);
int String_append_char(String* this, char c);
String* String_substring(const String* this, unsigned int start, unsigned int end);
unsigned int String_len(const String* this);
int String_is_equal(const String* str1, const String* str2);
int String_is_equal_c_str(const String* this, const char* c_str);
int String_find(const String* haystack, const String* needle, unsigned int occurrence);
int String_find_c_str(const String* haystack, const char* needle, unsigned int occurrence);
void String_shrink_to_fit(String* this);
List* String_split(String* this, const char* delim);


/* StringIterator methods */
StringIterator* StringIterator_new(String* str);
void StringIterator_destroy(StringIterator* this);
int StringIterator_index(const StringIterator* this);
char StringIterator_peak(const StringIterator* this);
char StringIterator_next(StringIterator* this);
char StringIterator_prev(StringIterator* this);
void StringIterator_reset(StringIterator* this);
void StringIterator_jump_to_last(StringIterator* this);
void StringIterator_jump_to(StringIterator* this, unsigned int index);
int StringIterator_modify(StringIterator* this, char c);


#endif