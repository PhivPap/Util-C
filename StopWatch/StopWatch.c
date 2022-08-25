#include "StopWatch.h"
#include <stdlib.h>
#include <time.h>

struct StopWatch {
    struct timespec check_point;
    double prev_elapsed_sec;
    unsigned char is_running;
};

StopWatch* StopWatch_new(){
    StopWatch* this = malloc(sizeof(StopWatch));
    if(!this)
        return NULL;
    clock_gettime(CLOCK_MONOTONIC, &this->check_point);
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
    struct timespec timer_now;
    clock_gettime(CLOCK_MONOTONIC, &timer_now);
    return this->prev_elapsed_sec + (double)(timer_now.tv_sec - this->check_point.tv_sec) + ((double)(timer_now.tv_nsec - this->check_point.tv_nsec)/ 1000000000.0);
}

void StopWatch_reset(StopWatch* this){
    clock_gettime(CLOCK_MONOTONIC, &this->check_point);
    this->prev_elapsed_sec = 0.0;
    this->is_running = 1;
}

void StopWatch_pause(StopWatch* this){
    if(!this->is_running)
        return;
    struct timespec timer_now;
    clock_gettime(CLOCK_MONOTONIC, &timer_now);
    this->prev_elapsed_sec += (double)(timer_now.tv_sec - this->check_point.tv_sec) + ((double)(timer_now.tv_nsec - this->check_point.tv_nsec)/ 1000000000.0);
    this->is_running = 0;
}

void StopWatch_resume(StopWatch* this){
    if(this->is_running)
        return;
    clock_gettime(CLOCK_MONOTONIC, &this->check_point);
    this->is_running = 1;
}

int StopWatch_is_running(StopWatch* this){
    return (int)this->is_running;
}