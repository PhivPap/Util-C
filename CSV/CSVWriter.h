#include <stdio.h>

/* opaque types */
typedef struct CSVWriter CSVWriter;

/* CSVWriter methods */
CSVWriter* CSVWriter_new(FILE* fp, char delim);
void CSVWriter_destroy(CSVWriter* csv_w);
void CSVWriter_set_escape_char(CSVWriter* csv_w, char escape);
void CSVWriter_append_to_row(CSVWriter* csv_w, const char* str);
void CSVWriter_next_row(CSVWriter* csv_w);
