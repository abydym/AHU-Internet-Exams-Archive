#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NUM_PROCESS 100
#define KEY         1234
#define ITERATIONS  1000000

struct sharedMem{
    int n;
    pthread_mutex_t mutex;
};

int main(int argc, const char ** argv){
    //create the shared memory
    int shmid;
    struct sharedMem * shmadd;

    if((shmid = shmget((key_t) KEY, sizeof(struct sharedMem), IPC_CREAT | 0666))< 0){
        printf("Shmget failed!\n");
        exit(-1);
    }

    if((shmadd = (struct sharedMem *)shmat(shmid, NULL, 0)) == (void *)-1){
        printf("Shmat failed!\n");
        exit(-2);
    }

    //initialize the shared memory
    shmadd->n = 0;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shmadd->mutex, &attr);

    //create child process
    pid_t childProcessPID;

    for(int i = 0; i < NUM_PROCESS; i++){
        childProcessPID = fork();
        if(childProcessPID < 0){
            printf("Fork failed!\n");
            exit(-1);
        }

        //child process 
        else if(childProcessPID == 0){
            for(int i = 0; i < ITERATIONS; i++){
                pthread_mutex_lock(&shmadd->mutex);
                shmadd->n ++;
                if(shmadd->n % 100000 == 0)
                    printf("Process with PID %d: %d\n", getpid(), shmadd->n);
                pthread_mutex_unlock(&shmadd->mutex);          
            }

            shmdt(shmadd);
            exit(0);
        }
    }

    //wait for all child process to quit
    for(int i = 0; i < NUM_PROCESS; i++)
        wait(NULL);

    printf("\nResult: %d\n", shmadd->n);
    pthread_mutex_destroy(&shmadd->mutex);
    shmdt(shmadd);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}