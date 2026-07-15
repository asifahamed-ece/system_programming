#include <stdio.h>
#include <unistd.h>  // for fork(), getpid(), getppid()
#include <sys/types.h> // for the dtyoes like pid_t

int main()
{
    printf("--- Before fork() ---\n");
    printf("I am the Original process, My PID is %d\n", getpid());

    pid_t returnValue = fork();  // fork called, Creating New process

    printf("--- After Fork() ---\n");
    printf("My PID is %d, return value is %d\n", getpid(), returnValue);


    return 0;
}
