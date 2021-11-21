#include <stdio.h>
#include "String.h"

int main(int argc, const char** argv){
    String* str0 = String_new();
    String* str1;

    String_append_str(str0, str1 = String_new_copy("Hello"));
    String_append_char(str0, ' ');
    String_append_c_str(str0, "World!");
    String_append_str(str0, str0);
    

    printf("%s\n", String_data(str0));

    StringIterator* iter = StringIterator_new(str0);
    char c;
    while(c = StringIterator_next(iter)){
        printf("## %c\n", c);
    }
    StringIterator_prev(iter);
    while(c = StringIterator_prev(iter)){
        printf("%c", c);
    }
    StringIterator_destroy(iter);
    putc('\n', stdout);

    String_destroy(str0);
    String_destroy(str1);
}