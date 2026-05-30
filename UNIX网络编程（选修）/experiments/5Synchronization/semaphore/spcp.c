#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NBUFF       5
#define NITERATIONS 20

struct sharedData{
    int   buff[NBUFF];
    int   front, rear;
    sem_t mutex, nempty, nstored;
};

struct sharedData sd;

static void * producer(void * arg);
static void * consumer(void * arg);

int main(int argc, const char ** argv){
    pthread_t tid_producer, tid_consumer;
    
    sd.front = 0;
    sd.rear  = 0;
    sem_init(&sd.mutex, 0, 1);
    sem_init(&sd.nempty, 0, NBUFF);
    sem_init(&sd.nstored, 0, 0);

    pthread_create(&tid_producer, NULL, producer, NULL);
    pthread_create(&tid_consumer, NULL, consumer, NULL);

    pthread_join(tid_producer, NULL);
    pthread_join(tid_consumer, NULL);

    sem_destroy(&sd.mutex);
    sem_destroy(&sd.nempty);
    sem_destroy(&sd.nstored);
}

static void * producer(void * arg){
    for(int i = 0; i < NITERATIONS; i++){
        sem_wait(&sd.nempty);
        sem_wait(&sd.mutex);

        sd.buff[sd.rear] = i;
        printf("%-3d: the producer thread put a product %-3d into the buffer %d\n", i, i, sd.rear);
        sd.rear = (sd.rear + 1) % NBUFF;
        
        sem_post(&sd.mutex);
        sem_post(&sd.nstored);

        sleep(5);
    }

    return NULL;
}

static void * consumer(void * arg){
    for(int i = 0; i < NITERATIONS; i++){
        sem_wait(&sd.nstored);
        sem_wait(&sd.mutex);

        printf("%-3d: the consumer thread consume a product %-3d from the buffer %d\n", i, sd.buff[sd.front], sd.front);
        sd.front = (sd.front + 1) % NBUFF;

        sem_post(&sd.mutex);
        sem_post(&sd.nempty);

        sleep(2);
    }
    
    return NULL;
}
