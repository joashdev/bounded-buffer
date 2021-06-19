//  Link to defense: http://bit.ly/zamudio_125lab2
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int bufferHasContent = 0;
int putIndex = 0;
int getIndex = 0;

// pthreads
pthread_mutex_t bufferMutex;
pthread_cond_t bufferFull;
pthread_cond_t bufferEmpty;

void* producer(void* producerId){
    long id = (long) producerId;

    while(1) {
        // produce
        int produced = (rand()%9)+1;
       // sleep(1);

        pthread_mutex_lock(&bufferMutex);

        //check if there is a space
        while(bufferHasContent>=BUFFER_SIZE) { pthread_cond_wait(&bufferEmpty, &bufferMutex); } //explain cond_wait
        
        // put the product to the buffer
        buffer[putIndex] = produced;
        printf("producer %ld produced %d at %d\n", id, buffer[putIndex], putIndex);
        putIndex = (putIndex+1)%BUFFER_SIZE;
        bufferHasContent++;

        pthread_cond_signal(&bufferFull);
        pthread_mutex_unlock(&bufferMutex);

    }
}

void* consumer(void* consumerId) {
    long id = (long) consumerId;
    while(1){
       pthread_mutex_lock(&bufferMutex);

       // check if there is a content in the buffer
       while(bufferHasContent<=0) { pthread_cond_wait(&bufferFull, &bufferMutex); }

       // get consumption
       int consumed = buffer[getIndex];
       printf("consumer %ld consumed %d at %d\n", id, consumed, getIndex);
       getIndex = (getIndex+1)%BUFFER_SIZE;
       bufferHasContent--;

       pthread_cond_signal(&bufferEmpty);
       pthread_mutex_unlock(&bufferMutex);

       //sleep(1);
    }
}


int main(int argc, char *argv[] )  {  
    srand(time(NULL));
        
    // parse argv
    int prodNum = strtol(argv[1],NULL,0);
    int consNum = strtol(argv[2],NULL,0);
    int threadSize = prodNum + consNum;
    long threadIds[threadSize];

    // init pthreads variables
    pthread_t thr[threadSize];
    pthread_mutex_init(&bufferMutex, NULL);
    pthread_cond_init(&bufferFull, NULL);
    pthread_cond_init(&bufferEmpty, NULL);

    // 0 1 2 3 4 5 6 7 8 9
    // prodNum = 5; consNum=5;
    // create threads
    for(int i = 0; i < threadSize; i++) {
        if (i<prodNum){
            threadIds[i] = i;
            if (pthread_create(&thr[i], NULL, &producer, (void*) threadIds[i]) != 0) {
                perror("Producer thread not created");
            }
        } else {
            threadIds[i] = i- prodNum;
            if (pthread_create(&thr[i], NULL, &consumer, (void*) threadIds[i]) != 0) {
                perror("Consumer thread not created");
            }
        }
    }
    // join threads
    for(int i = 0; i < threadSize; i++) {                                                                                                             
        if (pthread_join(thr[i], NULL) != 0) {
            perror("Thread not joined");
        }
    }

    // destroy pthreads variables
    pthread_mutex_destroy(&bufferMutex);
    pthread_cond_destroy(&bufferFull);
    pthread_cond_destroy(&bufferEmpty);
    return 0;
}
