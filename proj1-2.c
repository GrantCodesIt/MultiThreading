#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
//generate RandomPoints with 1 thread
// 1 thread will count total # of points inside circle
// 1 thread total # goes into global variabel
// parent thread calculate and output value of pi



int main() {

    int i,n;
    n = 5;
    time_t t;
    srand((unsigned) time(&t));

    for( i = 0 ; i < n ; i++ ) {
      printf("%d\n", rand() % 50);
   }
    return(0);
}