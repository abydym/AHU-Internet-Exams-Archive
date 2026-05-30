#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>

#define SHARED_MEMORY_SIZE 1024
#define KEY                55312

int main(int argc, char ** argv){
    int shmid;
    char * shmadd;
    char buf[SHARED_MEMORY_SIZE];

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
        printf("Input message: ");
        bzero(buf, SHARED_MEMORY_SIZE);
        fgets(buf, SHARED_MEMORY_SIZE, stdin);
        buf[strlen(buf) - 1] = '\0';

        strcpy(shmadd, buf);

        if(strcmp(buf, "quit") == 0)
            break;
    }

    shmdt(shmadd);
    return 0;
}