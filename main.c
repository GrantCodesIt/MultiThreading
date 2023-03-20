#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10


char bufferChar[BUFFER_SIZE];
int add = 0;
int rem = 0;
int num = 0;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;	/* mutex lock for buffer */
pthread_cond_t c_cons=PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t c_prod=PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[])
{
    FILE *ptr = fopen("cse3320.txt", "r");
    if (ptr == NULL) {
        printf("File not opened");
    } 

    pthread_t tid1, tid2;
    /* create the threads; may be any number, in general */
	if (pthread_create(&tid1,NULL,producer,ptr) != 0) {
		fprintf (stderr, "Unable to create producer thread\n");
		exit (1);
	}
	if (pthread_create(&tid2,NULL,consumer,ptr) != 0) {
		fprintf (stderr, "Unable to create consumer thread\n");
		exit (1);
	}
	//pthread_create(&tid1,NULL,producer,ptr);
	//pthread_create(&tid2,NULL,producer,ptr);
	/* wait for created thread to exit */
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	printf ("Parent quiting\n");


}

void *producer(void *ptr) {
    printf("Inside Producer \n");
    char *ch;
    size_t n=0;
    int c = fgetc(ptr);
    ch = malloc(1000);

    /////////////////
    while ((c = fgetc(ptr)) != EOF) {
    pthread_mutex_lock(&m); //getlock

        while (num == BUFFER_SIZE) {
            pthread_cond_wait (&c_prod, &m);
    }

        bufferChar[add] = ch;
        add = (add +1) % BUFFER_SIZE;
        num++;
        ch = fgetc(ptr);
        pthread_mutex_unlock (&m);
		pthread_cond_signal (&c_cons);
		//printf("%c" , ch);
		//printf("%c" , bufferChar[add]);
    }
    printf("Producer Quiting\n");
    ;
    pthread_exit(0);
}
void *consumer(void *ptr) {
    printf("Inside Consumer\n");
    char chConsume;
    while (!feof(ptr)) {
        pthread_mutex_lock(&m);
        if (num < 0) exit(1); //underflow
        while(num ==0) {
            pthread_cond_wait (&c_cons, &m);
        }
        chConsume = bufferChar[rem];
        rem = (rem + 1) % BUFFER_SIZE;
        num = 0;
        pthread_mutex_unlock (&m);
		pthread_cond_signal (&c_prod);
        printf("%c" ,chConsume);

    }
    printf("Consumer Quiting\n");
    
}
