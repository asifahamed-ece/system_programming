#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define FIFONAME "/tmp/sensor_fifo"

int main()
{
    int fd;
    char buff[100];

    printf("[Reader]:  Opening FIFO for Reading...\n");

    // Open the FIFO for Reading.
    //This call will Block until another call opens the FIFO for Writing.
    fd = open(FIFONAME, O_RDONLY);
    if(fd == -1){
        perror("FIFO Open Failed!\n");
        return 1;
    }

    printf("[Reader]: Writer Connected! Data being Recieved...\n");

    // Reading the Contents from FIFO
    // This read blocks until data is available
    ssize_t bytes_read = read(fd, buff, sizeof(buff));
    if(bytes_read == -1){
        perror("FIFO Read Failed~\n");
        close(fd);
        return 1;
    }

    printf("[Reader]: Successfully Read %zd bytes from FIFO\n", bytes_read);
    printf("[Reader]: Received Message : %s\n", buff);

    printf("\nClosing FIFO\n");
    close(fd);

    printf("[Reader]: Removing FIFO from FileSystem\n");
    if(unlink(FIFONAME) == -1){
        perror("Error Removing FIFO\n");
        return 1;
    }

    printf("[Reader]: Cleanup Complete! Exiting...\n");
    return 0;

}
