#include <stdio.h>
#include <stdlib.h>
#include "Json.h"

// gcc DemoRead.c Json.c ../List/List.c ../HT/HashTable.c

const char* json_file_path = "./DemoInput.json";

void gracefully_exit(const char* msg){
    printf("%s\n", msg);
    exit(1);
}

void print_usernames_unsafe(JsonObj* json){
    JArrayIter* users_iter = JArrayIter_new(JsonObj_dict_get(json, "users"));
    JsonObj* user;
    while(user = JArrayIter_next(users_iter))
        printf("%s\n", JsonObj_get_string(JsonObj_dict_get(user, "username")));
    JarrayIter_destroy(users_iter);
}

void print_usernames_safe(JsonObj* json){
    JsonObj* users = JsonObj_dict_get(json, "users");
    if((!users) || (JsonObj_get_type(users) != JArray))
        gracefully_exit("Missing 'users' array in json.");
    JArrayIter* users_iter = JArrayIter_new(users);
    JsonObj* user;
    while(user = JArrayIter_next(users_iter)){
        if(JsonObj_get_type(user) != JDict)
            gracefully_exit("Array 'users' contains non-dictionary objects.");
        JsonObj* username = JsonObj_dict_get(user, "username");
        if((!username) || (JsonObj_get_type(username) != JString))
            gracefully_exit("'User' is missing attribute 'username' of type string.");
        printf("%s\n", JsonObj_get_string(username));
    }
    JarrayIter_destroy(users_iter);
}

void main(){
    JsonObj* json = JsonObj_parse_file(json_file_path);
    if(!json){
        printf("Failed to parse '%s'\n", json_file_path);
        return;
    }

    //print_usernames_safe(json);
    print_usernames_unsafe(json);
    JsonObj_deep_destroy(json);
}