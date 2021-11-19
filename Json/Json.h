#ifndef _MY_JSON_
#define _MY_JSON_

#include <stdio.h>

/* Opaque types */
typedef struct JsonObj JsonObj;
typedef struct JArrayIter JArrayIter;
typedef struct JDictIter JDictIter;

/* Types */
typedef enum JsonType { JString, JNumber, JBool, JArray, JDict, JNull } JsonType;

typedef struct JDictPair {
    const char* key;
    JsonObj* jsonobj;
} JDictPair;

/* JsonObj parsers and printers */
JsonObj* JsonObj_parse_file(const char* json_file_path);
JsonObj* JsonObj_parse_string(const char* sp);
void JsonObj_fprint(const JsonObj* this, FILE* fp);
char* JsonObj_sprint(const JsonObj* this);

/* JsonObj constructors and seters */
JsonObj* JsonObj_new_string(const char* str);
JsonObj* JsonObj_new_number(double num);
JsonObj* JsonObj_new_bool(int boolean);
JsonObj* JsonObj_new_array(void);
JsonObj* JsonObj_new_dict(void);
JsonObj* JsonObj_new_null(void);

/* JsonObj array modifiers */
int JsonObj_array_append(JsonObj* jarray, JsonObj* elem);
int JsonObj_array_insert(JsonObj* jarray, JsonObj* elem, unsigned int index);
int JsonObj_array_elem_deep_destroy(JsonObj* jarray, unsigned int index);
int JsonObj_array_elem_destroy(JsonObj* jarray, unsigned int index);
void JsonObj_array_deep_clear(JsonObj* jarray);
void JsonObj_array_clear(JsonObj* jarray);

/* JsonObj dictionary modifiers */
int JsonObj_dict_add(JsonObj* jdict, const char* key, JsonObj* value);
int JsonObj_dict_elem_deep_destroy(JsonObj* jdict, const char* key);
int JsonObj_dict_elem_destroy(JsonObj* jdict, const char* key);
void JsonObj_dict_deep_clear(JsonObj* jdict);
void JsonObj_dict_clear(JsonObj* jdict);

/*JsonObj destructors */
void JsonObj_destroy(JsonObj* this);
void JsonObj_deep_destroy(JsonObj* this);

/* JsonObj geters */
JsonType JsonObj_get_type(const JsonObj* jobj);
const char* JsonObj_get_string(const JsonObj* jstring);
double JsonObj_get_number(const JsonObj* jnumber);
int JsonObj_get_bool(const JsonObj* jbool);
unsigned int JsonObj_array_size(const JsonObj* jarray);
JsonObj* JsonObj_array_get(const JsonObj* jarray, unsigned int index);
unsigned int JsonObj_dict_size(const JsonObj* jdict);
JsonObj* JsonObj_dict_get(const JsonObj* jdict, const char* key);

/* JsonObj iterators */
/*      JArrayIter functions */
JArrayIter* JArrayIter_new(const JsonObj* jarray);
JsonObj* JArrayIter_peak(JArrayIter* this);
JsonObj* JArrayIter_next(JArrayIter* this);
void JarrayIter_reset(JArrayIter* this);
void JarrayIter_destroy(JArrayIter* this);

/*      JDictIter */
JDictIter* JDictIter_new(const JsonObj* jdict);
JDictPair* JDictIter_peak(JDictIter* this);
JDictPair* JDictIter_next(JDictIter* this);
void JDictIter_reset(JDictIter* this);
void JDictIter_destroy(JDictIter* this);

#endif