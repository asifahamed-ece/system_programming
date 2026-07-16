#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

int main()
{
    printf("--- DUP DEMO: Redirecting stdout to a file ---\n\n");
    // 1. Copy the fd of Stdout and Save it for Recovering later
    int save_stdout = dup(STDOUT_FILENO);  // For Re-enabling when done
    printf("[1] Original Stdout Saved as fd %d\n", save_stdout);
    printf("Saved STDout %d\n", save_stdout);

    // 2. Duplicating the New file Descriptor and making it as fd 1
    int file_fd = open("asifsOut.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644 );
    if (file_fd == -1){
        perror("File Open Failed\n");
        return 1;
    }
    printf("Opened asifsOut.txt as fd : %d\n", file_fd);

    // 3. Now we make our file_fd as the fd = 1 and close our file_fd.
    // Our filr can be accessed just by using the fd = 1 (prev. it was for stdout)
    dup2(file_fd, STDOUT_FILENO);  // file_fd is copied to stdout.

    printf("[3] this msg goes into the File, Not in Terminal\n");
    printf("[4] ANother message Storing inside the asifsOut.txt file instead of Stdout\n");

    // 4. Restoring the Stdout's fd as 1.
    dup2(save_stdout, STDOUT_FILENO);

    printf("[5] This message comes back in the stdout which is the Terminal\n");
    printf("Restored Stdout %d\n", save_stdout);

    // 5. Cleanup the file desciptors opened before leaving.
    close(file_fd);
    close(save_stdout);

    return 0;

}