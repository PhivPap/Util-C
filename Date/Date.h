#ifndef _MY_DATE_
#define _MY_DATE_

/* Opaque types */
typedef struct Date Date;

/* Visible types */
typedef struct DateInfo {
    unsigned short ms, sec, min, hour, day, month, year;
    const char* week_day;
    const char* month_name;
} DateInfo;

/* Date functions */
Date* Date_new();
// TODO: more constructors
void Date_destroy(Date* this);
DateInfo Date_info(Date* this);

#endif