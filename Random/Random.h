#ifndef _MY_RAND_
#define _MY_RAND_
// [min, max]

int random_int(int min, int max);
char random_char(char min, char max);
double random_double(double min, double max);
char* random_string(unsigned int length);
char* random_string_char_range(unsigned int length, char min, char max);


#endif