#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define FIFONAME "/tmp/sensor_fifo"

int main()
{
    int fd;     // file descriptor for Rd and Wr
    char *msg = "Temperature : 28.7C, Humidity: 89%";
    printf("[Writer]: Created named Pipe (FIFO) at %s\n", FIFONAME);

    // Create the FIFO with read/write permissions for everyone
    // If it already exists, mkfifo returns -1 with errno=EEXIST (we ignore this)
    if(mkfifo(FIFONAME, 0666) == -1){
        if(errno != EEXIST){
            perror("mkfifo failed!\n");
            return 1;
        }
        printf("[Writer]: FIFO already exists. Continuing...\n");
    }

    printf("[Writer]: Opening FIFO for Writing...\n");
    // Upcoming FIFO Opening will Blocks until another Process opens it for Reading.
    fd = open(FIFONAME, O_WRONLY);
    if(fd == -1){
        perror("FIFO Open Failed!\n");
        return 1;
    }

    printf("[Writer]: Reader Connected! Data Starting to be Written...\n");

    ssize_t bytes_written = write(fd, msg, strlen(msg) + 1);
    if (bytes_written == -1){
        perror("FIFO Write Failed\n");
        return 1;
    }

    printf("[Writer]: Successfully Wrote %zd bytes. Closing FIFO\n", bytes_written);
    close(fd);

    return 0;
}