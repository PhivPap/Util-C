#include "Date.h"

typedef unsigned int uint;

struct Date {
    DateInfo* date_info;
    // ...
};


Date* Date_new(){
    Date* this = malloc(sizeof(Date));
    // ...
    return this;
}

void Date_destroy(Date* this){

}

double Date_diff_sec(Date* clock1, Date* clock2){

}