#include <stdio.h>
#include "Timer.h"

void slow_function(){
    long long unsigned int j = 0;
    for(long long unsigned int i=0; i<1000000000; i++){
        j += 1;
    }
}

int main(int argc, const char** argv){
    Timer* timer = Timer_new();
    Timer* timer2 = Timer_new();

    slow_function();
    Timer_pause(timer2);
    slow_function();

    printf("timer: %fs\n", Timer_elapsed_sec(timer));
    printf("timer2: %fs\n", Timer_elapsed_sec(timer2));

    Timer_destroy(timer);
    Timer_destroy(timer2);
}