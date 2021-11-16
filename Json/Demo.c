#include <stdio.h>

#include "Json.h"

void main(){
    // TODO print: escape " & string version.



    // JsonObj* json = JsonObj_new_dict();
    // JsonObj* json2 = JsonObj_new_dict();
    // JsonObj_dict_add(json2, "key0", JsonObj_new_string("a string!"));

    // JsonObj* json_list = JsonObj_new_array();
    // JsonObj_array_append(json_list, JsonObj_new_bool(1));
    // JsonObj_array_append(json_list, JsonObj_new_bool(0));
    // JsonObj_array_append(json_list, json2);

    // JsonObj_dict_add(json, "key0", JsonObj_new_string("a string!"));
    // JsonObj_dict_add(json, "key1", JsonObj_new_string("another str"));
    // JsonObj_dict_add(json, "key2", JsonObj_new_number(10.1234));
    // JsonObj_dict_add(json, "key3", JsonObj_new_number(-90.4321));
    // JsonObj_dict_add(json, "key4", json_list);
    // JsonObj_dict_add(json, "key5", JsonObj_new_null());

    

    
    // JsonObj_fprint(json, stdout);


    // JsonObj_deep_destroy(json);


    JsonObj* json = JsonObj_parse_string("{\"aaa\" : { \"test\" : 12332.231 } }");
    if(json){
        JsonObj_fprint(json, stdout);
        JsonObj_deep_destroy(json);
    }
    else {
        printf("Unlucky json\n");
    }
}


/*

    {
        "aaa" : true
    }
*/