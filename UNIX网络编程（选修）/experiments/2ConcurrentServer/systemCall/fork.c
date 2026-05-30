#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char ** argv){

    pid_t pid = fork();

    if(pid < 0){
        printf("fork failed\n");
        return -1;
    }

    else if (pid == 0){
        printf("I'm the child process, my pid is %d, my parent's pid is %d\n", getpid(), getppid());
    }

    else
        printf("I'm the parent process, my pid is %d, my child's pid is %d\n", getpid(), pid);

    return 0;
}