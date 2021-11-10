#include <stdio.h>
#include <stdlib.h>
#include "List.h"

char* int_to_str(int num){
    char* str = malloc(sizeof(char) * 20);
    sprintf(str, "%d", num);
    return str;
}

int main(int argc, const char** argv){
    List* list = List_new();
    for(unsigned int i=0; i<10; i++){
        List_append(list, (void*)int_to_str(i*2));
    }


    ListIterator* iter = ListIterator_new(list);
    void* data;
    while(data = ListIterator_next(iter)){
        printf("%s\n", (char*)data);
    }


    ListIterator_destroy(iter);
    List_destroy_free(list);
}