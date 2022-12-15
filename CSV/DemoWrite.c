#include "CSVWriter.h"

int main(int argc, const char* argv[]) {
    CSVWriter* csv_w = CSVWriter_new(stdout, ',');

    CSVWriter_append_to_row(csv_w, "hello");
    CSVWriter_append_to_row(csv_w, "world");
    CSVWriter_append_to_row(csv_w, "12345");
    CSVWriter_next_row(csv_w);

    CSVWriter_append_to_row(csv_w, "qwe,t");
    CSVWriter_append_to_row(csv_w, "yenah");
    CSVWriter_append_to_row(csv_w, "yesno");
    
    CSVWriter_destroy(csv_w);
    return 0;
}