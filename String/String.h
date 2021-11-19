/* Opaque types */
typedef struct String String;
typedef struct StringIterator StringIterator;

/* String methods */
String* String_new();
String* String_new_copy(const char* c_str);
void String_destroy(String* this);
const char* String_data(const String* this);
int String_append_str(String* this, const String* str);
int String_append_c_str(String* this, const char* c_str);
int String_append_char(String* this, char c);
String* String_substring(const String* this, unsigned int start, unsigned int end);
unsigned int String_len(const String* this);
void String_shrink_to_fit(String* this);


/* StringIterator */
StringIterator* StringIterator_new(String* str);
void StringIterator_destroy(StringIterator* this);
char StringIterator_peak(StringIterator* this);
char StringIterator_next(StringIterator* this);
char StringIterator_prev(StringIterator* this);
void StringIterator_reset(StringIterator* this);
void StringIterator_jump_to_last(StringIterator* this);
int StringIterator_jump_to(StringIterator* this, unsigned int index);
int StringIterator_modify(StringIterator* this, char c);