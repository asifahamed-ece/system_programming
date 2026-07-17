#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    int pipe_fd[2];  // 0 -> read end , 1 -> write end
    if (pipe(pipe_fd) == -1){
        perror("PIPE creation Failed!\n");
        return 1;
    }

    printf("One Way Communication Via a Pipe from Child to Parent process.\n");

    int fval = fork();
    if(fval == -1){
        perror("Fork Failed!\n");
        return 2;
    }
    
    if (fval == 0){
        // Close the Unused end first
        close(pipe_fd[0]);  // Closing read end
        int num;
        printf("[Child] : Enter an Integer ");
        scanf("%d", &num);

        if (write(pipe_fd[1], &num, sizeof(int)) == -1){
            perror("Write Failed!\n");
            return 3;    
        }
        close(pipe_fd[1]);
    }

    else if (fval > 0){
        // Parent process
        close(pipe_fd[1]); // Closing write pipe as we are only here to read
        int out;
        
        if( read(pipe_fd[0], &out, sizeof(int)) == -1) {
            perror("Read Failed!\n");
            return 4;
        }
        close(pipe_fd[0]);

        printf("Got an int \"%d\" from Child\n", out);
    }
    return 0;
}