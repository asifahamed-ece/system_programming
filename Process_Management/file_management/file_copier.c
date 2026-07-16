// This C code is for the Demonstration of custom file Read and 
// Copy contents using the System Calls implemented using POSIX standards.

#include <stdio.h>
#include <unistd.h> // has the read(), write() and close()
#include <fcntl.h>  // For open() and O_flags
#include <stdlib.h>
#include <sys/stat.h>   // For file permissions (mode_t)

#define MAX_BUFF_SIZE 1024  // Max Buffer upto 1KB

int main(int argc, char *argv[])
{
    // 1. Checking if the Arguments passed are Valid.
    if (argc != 3){
        printf("Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }
    // Lets do the Copy Logic

    int src_fd, dest_fd;
    char buff[MAX_BUFF_SIZE];
    ssize_t bytes_read = 0, bytes_written = 0;

    // 2. Read the SOurce File
    src_fd = open(argv[1], O_RDONLY);
    if(src_fd == -1){
        perror("Failed to Read Source File!\n");
        return 1;
    }

    // 3. Create/Open a Destination file and add Permissions if needed.
    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1){
        perror("Failed to Write Destination File!\n");
        close(src_fd);              // Since we had opened src_fd beforehand.
        return 1;
    }

    // 4. Copy the COntents
    while((bytes_read = read(src_fd, buff, MAX_BUFF_SIZE)) > 0)
    {   
        // Writing exact bytes read.
        bytes_written = write(dest_fd, buff, bytes_read);

        if(bytes_written != bytes_read){
            perror("Write Failed!\n");
            close(src_fd);
            close(dest_fd);
            return 1;
        }
    }

    // 5. Checking if the Copying Exited with an Error ?
    if (bytes_read == -1){
        perror("Exited with Copy Error!\n");
    }

    // 6. Close all the files Opened src and dest.
    close(src_fd);
    close(dest_fd);

    printf("Successfully Copied %s to %s\n", argv[1], argv[2]);

    return 0;
}

