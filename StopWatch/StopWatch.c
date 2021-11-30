#include "StopWatch.h"
#include <sys/time.h>
#include <stdlib.h>

struct StopWatch {
    struct timeval check_point;
    double prev_elapsed_sec;
    unsigned char is_running;
};

StopWatch* StopWatch_new(){
    StopWatch* this = malloc(sizeof(StopWatch));
    if(!this)
        return NULL;
    gettimeofday(&this->check_point, 0);
    this->prev_elapsed_sec = 0.0;
    this->is_running = 1;
    return this;
}

void StopWatch_destroy(StopWatch* this){
    free(this);
}

double StopWatch_elapsed_sec(StopWatch* this){
    if(!this->is_running)
        return this->prev_elapsed_sec;
    struct timeval timer_now;
    gettimeofday(&timer_now, 0);
    return this->prev_elapsed_sec + (double)(timer_now.tv_sec - this->check_point.tv_sec) + ((double)(timer_now.tv_usec - this->check_point.tv_usec)/ 1000000.0);
}

void StopWatch_reset(StopWatch* this){
    gettimeofday(&this->check_point, 0);
    this->prev_elapsed_sec = 0.0;
    this->is_running = 1;
}

void StopWatch_pause(StopWatch* this){
    if(!this->is_running)
        return;
    struct timeval timer_now;
    gettimeofday(&timer_now, 0);
    this->prev_elapsed_sec += (double)(timer_now.tv_sec - this->check_point.tv_sec) + ((double)(timer_now.tv_usec - this->check_point.tv_usec)/ 1000000.0);
    this->is_running = 0;
}

void StopWatch_resume(StopWatch* this){
    if(this->is_running)
        return;
    gettimeofday(&this->check_point, 0);
    this->is_running = 1;
}

int StopWatch_is_running(StopWatch* this){
    return (int)this->is_running;
}