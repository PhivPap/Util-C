#include <stdio.h>

#include "Json.h"

void main(){
    JsonObj* json = JsonObj_new_dict();

    JsonObj* json_list = JsonObj_new_list();
    JsonObj_list_append(json_list, JsonObj_new_bool(1));
    JsonObj_list_append(json_list, JsonObj_new_bool(0));

    JsonObj_dict_add(json, "key0", JsonObj_new_string("a string!"));
    JsonObj_dict_add(json, "key1", JsonObj_new_string("another str"));
    JsonObj_dict_add(json, "key2", JsonObj_new_number(10.1234));
    JsonObj_dict_add(json, "key3", JsonObj_new_number(-90.4321));
    JsonObj_dict_add(json, "key4", json_list);
    JsonObj_dict_add(json, "key5", NULL);

    

    
    JsonObj_fprint(json, stdout);


    // TODO - Recursive free
    
}