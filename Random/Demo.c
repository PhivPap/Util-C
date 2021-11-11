#include <stdio.h>
#include "Random.h"
#include <string.h>
void main(){
    // for(int i=0; i<100; i++){
    //     int random = random_range(10, 20);
    //     int random2 = random_range(10, 20);
    //     printf("%d - %d\n", random, random2);
    // }
    
    char* str = random_string(9);
    printf("%s %lu\n", str, strlen(str));
}