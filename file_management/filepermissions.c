#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For open()
#include <unistd.h>     // For close(), fchmod()
#include <sys/stat.h>   // For chmod() and mode_t

int main() {
    char *filename = "secure_data.txt";

    printf("--- FILE PERMISSIONS DEMO ---\n\n");

    // 1. Create a file with standard 0644 (rw-r--r--) permissions
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }
    write(fd, "Top Secret Embedded Sensor Data", 32);
    printf("1. Created file '%s' with 0644 (rw-r--r--) permissions.\n", filename);
    close(fd);

    // 2. Change permissions using chmod() (Make it executable for everyone: 0755)
    // 0755 = rwxr-xr-x
    if (chmod(filename, 0755) == 0) {
        printf("2. Changed permissions to 0755 (rwxr-xr-x) using chmod().\n");
    } else {
        perror("chmod failed");
    }
    // close(fd);

    // 3. Open the file again and use fchmod() (Make it strictly read-only: 0444)
    // 0444 = r--r--r--
    fd = open(filename, O_RDONLY);
    if (fd != -1) {
        if (fchmod(fd, 0444) == 0) {
            printf("3. Changed permissions to 0444 (r--r--r--) using fchmod().\n");
        } else {
            perror("fchmod failed");
        }
        close(fd);
    }

    printf("\n[Action Required] Run 'ls -l %s' in your terminal to see the final permissions!\n", filename);

    // Note: We intentionally DO NOT delete the file here so you can see it in 'ls -l'
    return 0;
}