#ifndef _MY_STOPWATCH_
#define _MY_STOPWATCH_

/* Opaque types */
typedef struct StopWatch StopWatch;

/* StopWatch functions */
StopWatch* StopWatch_new();
void StopWatch_destroy(StopWatch* this);
double StopWatch_elapsed_sec(StopWatch* this);
void StopWatch_reset(StopWatch* this);
void StopWatch_pause(StopWatch* this);
void StopWatch_resume(StopWatch* this);
int StopWatch_is_running(StopWatch* this);
double StopWatch_diff(StopWatch* sw0, StopWatch* sw1); // TODO



#endif