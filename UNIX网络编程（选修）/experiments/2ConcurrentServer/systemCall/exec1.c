#include <stdio.h>
#include <unistd.h>

int main(int argc, const char ** argv){

    printf("I'm exec1.c, my pid is %d\n", getpid());
    printf("calling exec function...\n");
    
    execl("./exec2", "", NULL);

    return 1;
}