#include <stdio.h>      // Standard C library for stdin, stdout and stderr stuffs
#include <unistd.h>     // unix Standard lib which had the fork(), exec() syscalls
#include <sys/types.h>  // for using pid_t 
#include <sys/wait.h>  // for waitpid() , wait until child exits safely
#include <stdlib.h>  // for exit()

int main()
{
    pid_t pid = fork();  // Creating a Fork Child process

    if(pid < 0){
        perror("Fork Failed!\n");
        exit(1);
    }
        // Adding args for passing into the Child Process.
    char* args[] = {"ls", "-la", NULL};   // NULL Tells where the Commands stop. (Imp !)

    if (pid == 0){
        printf("[Child %d]: I am Running... Replacing Myself with \"%s %s\" \n", getpid(), args[0], args[1]);

        execvp(args[0], args);   // Executing the ls command within the Child Process.  // also can be (args[0], args).

        perror("execvp() failed!\n");  // exec() are One way trips, they switch the process entirely to ls -lh, after that command.
        exit(1);
    }

    if (pid > 0){
        printf("[Parent]: I created the Child %d. Waiting for it to Finish it's Task...\n", pid);
        int status;             // a Variable for monitoring the Childs exit status
        waitpid(pid, &status, 0);   // This prevents the child from becoming a Zombie!

        if(WIFEXITED(status))
            printf("\nChild %d Completed it's Work with Status %d.\n", pid, WEXITSTATUS(status));// checking the status of CHild when It dies.
    }
    return 0;
}