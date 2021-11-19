#include "String.h"
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

struct String {
    const char* c_str;
    uint length, size;
};

struct StringIterator {
    String* string;
    uint index;
};

const uint INIT_SIZE = 20;

/* String methods */
String* String_new(){
    String* this = malloc(sizeof(String));
    if(!this)
        return NULL;
    this->c_str = malloc(INIT_SIZE);
    if(!this->c_str){
        free(this);
        return NULL;
    }
    this->size = INIT_SIZE;
    this->length = 0;
    return this;
}

String* String_new_copy(const char* c_str){
    String* this = malloc(sizeof(String));
    this->length = strlen(c_str);
    this->size = this->length + 1;
    if(!this)
        return NULL;
    this->c_str = malloc(this->size);
    if(!this->c_str){
        free(this);
        return NULL;
    }
    strcpy(this->c_str, c_str);
    return this;
}

void String_destroy(String* this){
    free(this->c_str);
    this->c_str = NULL;
    free(this);
}

static int String_expand(String* this){
    assert(0);
}

const char* String_data(const String* this){
    return this->c_str;
}

int String_append_str(String* this, const String* str){
    assert(0);
}

int String_append_c_str(String* this, const char* c_str){
    assert(0);
}

int String_append_char(String* this, char c){
    assert(0);
}

String* String_substring(const String* this, unsigned int start, unsigned int end){
    assert(0);
}

unsigned int String_len(const String* this){
    assert(0);
}

void String_shrink_to_fit(String* this){
    assert(0);
}


/* StringIterator */
StringIterator* StringIterator_new(String* str){
    assert(0); 
}

void StringIterator_destroy(StringIterator* this){
    assert(0);
}

char StringIterator_peak(StringIterator* this){
    assert(0);
}

char StringIterator_next(StringIterator* this){
    assert(0);    
}

char StringIterator_prev(StringIterator* this){
    assert(0);
}

void StringIterator_reset(StringIterator* this){
    assert(0);
}

void StringIterator_jump_to_last(StringIterator* this){
    assert(0);
}

int StringIterator_jump_to(StringIterator* this, unsigned int index){
    assert(0);
}

int StringIterator_modify(StringIterator* this, char c){
    assert(0);
}
