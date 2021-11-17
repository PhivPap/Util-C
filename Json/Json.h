#ifndef _MY_JSON_
#define _MY_JSON_

#include <stdio.h>

/* Opaque types */
typedef struct JsonObj JsonObj;
typedef struct JArrayIter JArrayIter;
typedef struct JDictIter JDictIter;

/* Visible types */
typedef enum JsonType { JString, JNumber, JBool, JArray, JDict, JNull } JsonType;

typedef struct JDictPair {
    const char* key;
    JsonObj* jsonobj;
} JDictPair;


/* JsonObj constructors and seters */
JsonObj* JsonObj_new_string(const char* str);
JsonObj* JsonObj_new_number(double num);
JsonObj* JsonObj_new_bool(int boolean);
JsonObj* JsonObj_new_array(void);
JsonObj* JsonObj_new_dict(void);
JsonObj* JsonObj_new_null(void);
JsonObj* JsonObj_parse_file(const char* json_file_path);
JsonObj* JsonObj_parse_string(const char* sp);
void JsonObj_destroy(JsonObj* this);
void JsonObj_deep_destroy(JsonObj* this);
int JsonObj_array_append(JsonObj* this, JsonObj* elem);
int JsonObj_dict_add(JsonObj* this, const char* key, JsonObj* value);
void JsonObj_fprint(const JsonObj* this, FILE* fp);

/* JsonObj iterators & geters */
JsonType JsonObj_get_type(const JsonObj* jobj);
const char* JsonObj_get_string(const JsonObj* jstring);
double JsonObj_get_number(const JsonObj* jnumber);
int JsonObj_get_bool(const JsonObj* jbool);
JsonObj* JsonObj_get_array_value(const JsonObj* jarray, unsigned int index);
JsonObj* JsonObj_get_dict_value(const JsonObj* jdict, const char* key);

/* JArrayIter functions */
JArrayIter* JArrayIter_new(const JsonObj* jarray);
JsonObj* JArrayIter_peak(JArrayIter* this);
JsonObj* JArrayIter_next(JArrayIter* this);
void JarrayIter_reset(JArrayIter* this);
void JarrayIter_destroy(JArrayIter* this);

/* JDictIter */
JDictIter* JDictIter_new(const JsonObj* jdict);
JDictPair* JDictIter_peak(JDictIter* this);
JDictPair* JDictIter_next(JDictIter* this);
void JDictIter_reset(JDictIter* this);
void JDictIter_destroy(JDictIter* this);

#endif