#ifndef _MY_TIMER_
#define _MY_TIMER_

/* Opaque types */
typedef struct Timer Timer;

/* Timer functions */
Timer* Timer_new();
void Timer_destroy(Timer* this);
double Timer_elapsed_sec(Timer* this);
void Timer_reset(Timer* this);
void Timer_pause(Timer* this);
void Timer_resume(Timer* this);
int Timer_is_running(Timer* this);
double Timer_diff(Timer* timer0, Timer* timer1); // TODO

/* Other functions */






#endif