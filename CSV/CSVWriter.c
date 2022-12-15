#include "CSVWriter.h"
#include <stdlib.h>
#include <stdbool.h>

struct CSVWriter {
    FILE* fp;
    char delim, escape;
    bool first_col;
};

/* CSVWriter methods */
CSVWriter* CSVWriter_new(FILE* fp, char delim) {
    if (fp == NULL)
        return NULL;

    CSVWriter*  csv_w = malloc(sizeof(struct CSVWriter));
    if (csv_w == NULL)
        return NULL;

    csv_w->fp = fp;
    csv_w->delim = delim;
    csv_w->escape = '\\';
    csv_w->first_col = true;
    return csv_w;
}

void CSVWriter_destroy(CSVWriter* csv_w) {
    if (!csv_w->first_col)
        putc_unlocked('\n', csv_w->fp);
    free(csv_w);
}

void CSVWriter_set_escape_char(CSVWriter* csv_w, char escape) {
    csv_w->escape = escape;
}

void CSVWriter_append_to_row(CSVWriter* csv_w, const char* str) {
    if (!csv_w->first_col)
        putc_unlocked(csv_w->delim, csv_w->fp);
    
    char c;
    while ((c = *str++) != '\0') {
        if (c == csv_w->delim || c == '\n' || c == csv_w->escape)
            putc_unlocked(csv_w->escape, csv_w->fp);
        putc_unlocked(c, csv_w->fp);
    }
    csv_w->first_col = false;
}

void CSVWriter_next_row(CSVWriter* csv_w) {
    putc_unlocked('\n', csv_w->fp);
    csv_w->first_col = true;
}
