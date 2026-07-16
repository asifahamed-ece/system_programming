#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // 1. Create a file and write 16 bytes: "0123456789ABCDEF"
    int fd = open("lseek_test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    write(fd, "0123456789ABCDEF", 16); 

    char buf[5] = {0}; // Buffer to hold read data

    // 2. SEEK_SET: Jump to an absolute position (offset 5 -> character '5')
    lseek(fd, 5, SEEK_SET); 
    read(fd, buf, 2);
    printf("1. Jumped to offset 5. Read: '%s'\n", buf); // Prints "56"

    // 3. SEEK_CUR: Jump relative to current position (go back 2 bytes)
    lseek(fd, -2, SEEK_CUR); 
    read(fd, buf, 2);
    printf("2. Went back 2 bytes. Read: '%s'\n", buf); // Prints "56" again

    // 4. SEEK_END: Jump relative to the end (4 bytes before the end -> 'C')
    lseek(fd, -4, SEEK_END); 
    read(fd, buf, 4);
    printf("3. Jumped to 4 bytes before end. Read: '%s'\n", buf); // Prints "CDEF"

    // 5. TRICK: Find file size without using stat()
    off_t size = lseek(fd, 0, SEEK_END);
    printf("4. File size is: %ld bytes\n", size); // Prints 16

    close(fd);
    remove("lseek_test.txt"); // Clean up
    return 0;
}