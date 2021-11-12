#include <stdio.h>
#include "StopWatch.h"

void slow_function(){
    long long unsigned int j = 0;
    for(long long unsigned int i=0; i<1000000000; i++){
        j += 1;
    }
}

int main(int argc, const char** argv){
    StopWatch* stopwatch = StopWatch_new();
    StopWatch* stopwatch2 = StopWatch_new();

    slow_function();
    StopWatch_pause(stopwatch2);
    slow_function();

    printf("stopwatch: %fs\n", StopWatch_elapsed_sec(stopwatch));
    printf("stopwatch2: %fs\n", StopWatch_elapsed_sec(stopwatch2));

    StopWatch_destroy(stopwatch);
    StopWatch_destroy(stopwatch2);
}