#ifndef _MY_JSON_
#define _MY_JSON_

#include <stdio.h>


/* Opaque types */
typedef struct JsonObj JsonObj;

/* JsonObj functions*/
JsonObj* JsonObj_new_string(const char* str);
JsonObj* JsonObj_new_number(double num);
JsonObj* JsonObj_new_bool(int boolean);
JsonObj* JsonObj_new_array(void);
JsonObj* JsonObj_new_dict(void);
JsonObj* JsonObj_parser(FILE* fp);

void JsonObj_destroy(JsonObj* this);
void JsonObj_deep_destroy(JsonObj* this);

int JsonObj_array_append(JsonObj* this, JsonObj* elem);
int JsonObj_dict_add(JsonObj* this, const char* key, JsonObj* value);
void JsonObj_fprint(const JsonObj* this, FILE* fp);





#endif