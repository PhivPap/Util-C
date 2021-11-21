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
    int index;
};

const uint DEF_SIZE = 20;               /* used by empty constructor */
const uint DEF_EXPAND_ADDED_LEN = 100;  /* used when resizing string*/

/* String methods */
String* String_new(){
    String* this = malloc(sizeof(String));
    if(!this)
        return NULL;
    this->c_str = malloc(DEF_SIZE);
    if(!this->c_str){
        free(this);
        return NULL;
    }
    this->size = DEF_SIZE;
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
    strncpy(this->c_str, c_str, this->size);
    return this;
}

String* String_clone(const String* this){
    String* clone = malloc(sizeof(String));
    if(!clone)
        return NULL;
    clone->c_str = String_clone_data(this);
    if(!clone->c_str){
        free(clone);
        return NULL;
    }
    clone->length = this->length;
    clone->size = this->length + 1;
    return clone;
}

void String_destroy(String* this){
    free(this->c_str);
    this->c_str = NULL;
    free(this);
}

static int String_expand(String* this, unsigned int new_len){
    uint new_size = new_len + 1 + DEF_EXPAND_ADDED_LEN;
    char* new_c_str = malloc(new_size);
    if(!new_c_str)
        return -1;
    strncpy(new_c_str, this->c_str, new_size);
    free(this->c_str);
    this->size = new_len;
    this->c_str = new_c_str;
    return 0;
}

const char* String_data(const String* this){
    return this->c_str;
}

char* String_clone_data(const String* this){
    char* clone = malloc(this->length + 1);
    if(!clone)
        return NULL;
    strncpy(clone, this->c_str, this->size);
    return clone;
}

int String_append_str(String* this, const String* str){
    uint combined_len = this->length + str->length;
    if(combined_len >= this->size)
        if(String_expand(this, combined_len) == -1)
            return -1;
    if(this == str){
        uint tmp_size = str->length + 1;
        char* tmp = malloc(tmp_size);
        if(!tmp)
            return -1;
        strncpy(tmp, str->c_str, tmp_size);
        strncat(this->c_str, tmp, tmp_size);
        free(tmp);
    }
    else
        strncat(this->c_str, str->c_str, this->size);
    return 0;
}

int String_append_c_str(String* this, const char* c_str){
    uint c_str_len = strlen(c_str);
    uint combined_len = this->length + c_str_len;
    if(combined_len >= this->size)
        if(String_expand(this, combined_len) == -1)
            return -1;
    uint tmp_size = c_str_len + 1;
    char* tmp = malloc(tmp_size);
    if(!tmp)
        return -1;
    strncpy(tmp, c_str, tmp_size);
    strncat(this->c_str, tmp, tmp_size);
    free(tmp);
    return 0;
}

int String_append_char(String* this, char c){
    uint combined_len = this->length + 1;
    if(combined_len >= this->size)
        if(String_expand(this, combined_len) == -1)
            return -1;
    this->c_str[this->length++] = c;
    return 0;
}

String* String_substring(const String* this, unsigned int start, unsigned int end){
    assert(0);
}

unsigned int String_len(const String* this){
    return this->length;
}

void String_shrink_to_fit(String* this){
    uint new_size = this->length + 1;
    realloc(this->c_str, new_size);
    this->size = new_size;
}


/* StringIterator */
StringIterator* StringIterator_new(String* str){
    StringIterator* this = malloc(sizeof(StringIterator));
    if(!this)
        return NULL;
    this->string = str;
    this->index = 0;
    return this;
}

void StringIterator_destroy(StringIterator* this){
    free(this);
}

int StringIterator_index(const StringIterator* this){
    return this->index;
}

char StringIterator_peak(const StringIterator* this){
    if(this->index < 0)
        return 0;
    return this->string->c_str[this->index];
}

char StringIterator_next(StringIterator* this){
    char ret_val;
    if(this->index < 0)
        ret_val = 0;
    else
        ret_val = this->string->c_str[this->index];
    if(this->index < this->string->length)
        this->index++;
    return ret_val;
}

char StringIterator_prev(StringIterator* this){
    char ret_val;
    if(this->index < 0)
        ret_val = 0;
    else {
        ret_val = this->string->c_str[this->index];
        this->index--;
    }
    return ret_val;
}

void StringIterator_reset(StringIterator* this){
    this->index = 0;
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
