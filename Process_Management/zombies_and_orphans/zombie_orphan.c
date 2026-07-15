#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{
    printf("=== DEMONSTRATION 1: ZOMBIE PROCESS ===\n");
    printf("Parent PID: %d\n", getpid());
    pid_t pid1 = fork();

    if (pid1 == 0){
        printf("Child of %d: I am Exiting Now. I (%d) will become Zombie Soon!\n", getppid(), getpid());
        exit(0);
    }
    if (pid1 > 0){
        printf("{Zombie Parent} : My CHild %d Died. I am sleeping for 5seconds, Open terminal and type 'ps -l'\n", pid1);
        sleep(5);

        // Lets Clean the Zombie process by reaping...
        waitpid(pid1, NULL, 0);
        printf("{Parent}: Reaped the Zombie, Moving to Orphan Demo\n");
    }

    printf("=== DEMONSTRATION 2: ORPHAN PROCESS ===\n");
    pid_t pid2 = fork();

    if (pid2 > 0){
        printf("[orphans's parent]: I am Gonna Die Now to Make my Child %d orphan\n", pid2);
        exit(0);
    }
    else{
        printf("[Orphan Child %d] My parent is PID %d. I am sleeping...\n", getpid(), getppid());
        sleep(3);
        
        // Sleeping Child wakes up and sees its Parent had died and he has been adopted by init (systemd) PID=1
        printf("[Orphan Child %d]: I woke Up! My Parent has Died. I am Adopted by init (PID : %d)\n", getpid(), getppid());
        exit(0);
    }
    return 0;
}
