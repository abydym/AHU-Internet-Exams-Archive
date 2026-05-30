#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#define KEY     55312
#define ROUNDS  1000000

int main(int argc, char ** argv){
    pid_t pid = fork();
    if(pid < 0){
        printf("forl failed\n");
        return -1;
    }

    //child process
    else if(pid == 0){
        int shmid;
        int * shmadd;

        //create or open the shared memory
        shmid = shmget((key_t)KEY, sizeof(int), IPC_CREAT | 0666);
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

        for(int i = 0; i < ROUNDS; i++)
            * shmadd = * shmadd + 1;

        shmdt(shmadd);    
    }

    //parent process
    else{
        int shmid;
        int * shmadd;

        //create or open the shared memory
        shmid = shmget((key_t)KEY, sizeof(int), IPC_CREAT | 0666);
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

        for(int i = 0; i < ROUNDS; i++)
            * shmadd = * shmadd + 1;

        wait(NULL);
        
        printf("Result: %d\n", *shmadd);

        shmdt(shmadd);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
