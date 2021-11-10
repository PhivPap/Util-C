#include "Timer.h"
#include <sys/time.h>
#include <stdlib.h>

struct Timer {
    struct timeval check_point;
    double prev_elapsed_sec;
    unsigned char is_running;
};

Timer* Timer_new(){
    Timer* this = malloc(sizeof(Timer));
    if(!this)
        return NULL;
    gettimeofday(&this->check_point, 0);
    this->prev_elapsed_sec = 0.0;
    this->is_running = 1;
    return this;
}

void Timer_destroy(Timer* this){
    free(this);
}

double Timer_elapsed_sec(Timer* this){
    if(!this->is_running)
        return this->prev_elapsed_sec;
    struct timeval timer_now;
    gettimeofday(&timer_now, 0);
    return this->prev_elapsed_sec + (double)(timer_now.tv_sec - this->check_point.tv_sec) + ((double)(timer_now.tv_usec - this->check_point.tv_usec)/ 1000000.0);
}

void Timer_reset(Timer* this){
    gettimeofday(&this->check_point, 0);
    this->prev_elapsed_sec = 0.0;
    this->is_running = 1;
}

void Timer_pause(Timer* this){
    if(!this->is_running)
        return;
    struct timeval timer_now;
    gettimeofday(&timer_now, 0);
    this->prev_elapsed_sec += (double)(timer_now.tv_sec - this->check_point.tv_sec) + ((double)(timer_now.tv_usec - this->check_point.tv_usec)/ 1000000.0);
    this->is_running = 0;
}

void Timer_resume(Timer* this){
    if(this->is_running)
        return;
    gettimeofday(&this->check_point, 0);
    this->is_running = 1;
}

int Timer_is_running(Timer* this){
    if(this->is_running)
        return 1;
    return 0;
}