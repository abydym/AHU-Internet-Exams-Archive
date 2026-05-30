#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHARED_MEMORY_SIZE 1024
#define KEY                55312

int main(int argc, char ** argv){
    int shmid;
    char * shmadd;

    //create or open the shared memory
    shmid = shmget((key_t)KEY, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    if(shmid == -1){
        printf("shmget failed!\n");
        return -1;
    }

    //map the shared memory
    shmadd = shmat(shmid, NULL, 0);
    if(shmadd < 0){
        printf("shmadd failed!\n");
        return -1;
    }

    while(1){
        printf("Message in the shared memory: %s\n", shmadd);
        if(strcmp(shmadd, "quit") == 0)
            break;

        sleep(5);
    }

    shmdt(shmadd);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}