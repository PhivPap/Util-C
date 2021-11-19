#include <stdio.h>
#include "Json.h"

// gcc DemoWrite.c Json.c ../List/List.c ../HT/HashTable.c

void main(){
    //TODO print: escape " & string version.

    JsonObj* json = JsonObj_new_dict();
    JsonObj* json2 = JsonObj_new_dict();
    JsonObj_dict_add(json2, "key0", JsonObj_new_string("some string"));

    JsonObj* json_list = JsonObj_new_array();
    JsonObj_array_append(json_list, JsonObj_new_bool(1));
    JsonObj_array_append(json_list, JsonObj_new_bool(0));
    JsonObj_array_append(json_list, json2);

    JsonObj_dict_add(json, "key0", JsonObj_new_string("a string!"));
    JsonObj_dict_add(json, "key1", JsonObj_new_string("another str"));
    JsonObj_dict_add(json, "key2", JsonObj_new_number(10.1234));
    JsonObj_dict_add(json, "key3", JsonObj_new_number(-90.4321));
    JsonObj_dict_add(json, "key4", json_list);
    JsonObj_dict_add(json, "key5", JsonObj_new_null());


    
    JsonObj_fprint(json, stdout);

    JsonObj* json3 = JsonObj_dict_get(json, "key4");
    JsonObj_array_deep_clear(json3);

    JsonObj_fprint(json, stdout);

    JsonObj_dict_elem_destroy(json, "key0");

    JsonObj_fprint(json, stdout);

    JsonObj_deep_destroy(json);
}