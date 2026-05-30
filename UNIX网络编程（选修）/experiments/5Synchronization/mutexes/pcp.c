#include <stdio.h>
#include <pthread.h>

#define N_PRODUCER_THREADS 100
#define N_ITEMS            1000000

int gv_item;

pthread_mutex_t mutex;

static void * childThread(void * arg);

int main(int argc, const char ** argv){

    pthread_mutex_init(&mutex, NULL);

    pthread_t tid[N_PRODUCER_THREADS];

    for(int i = 0; i < N_PRODUCER_THREADS; i++)
        pthread_create(&tid[i], NULL, childThread, NULL);

    int itemCount = 0;
    for(int i = 0; i < N_ITEMS * N_PRODUCER_THREADS; i++){
        pthread_mutex_lock(&mutex);
        if(gv_item > 0){
            gv_item --;
            itemCount ++;
        }
        pthread_mutex_unlock(&mutex);
    }
    
    for(int i = 0; i< N_PRODUCER_THREADS; i++)
        pthread_join(tid[i], NULL);

    printf("itemCount: %d | gv_item:%d\n", itemCount, gv_item);

    return 0;
}

static void * childThread(void * arg){
    for(int i = 0; i < N_ITEMS; i++){
        pthread_mutex_lock(&mutex);
        gv_item ++;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
