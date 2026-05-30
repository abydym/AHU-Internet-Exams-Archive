#include <stdio.h>
#include <unistd.h>

int main(int argc, const char ** argv){

    printf("I'm exec2.c, my pid is %d\n", getpid());

    return 1;
}