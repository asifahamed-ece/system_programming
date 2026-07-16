#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[])
{   
    if(argc != 2){  // Only read 2 args from CLI
        printf("Usage: %s <file_path>\n", argv[0]);
        return 1;
    }
    struct stat file_info;

    // Check if the File Exists and gather its stats if Present.
    if (stat(argv[1], &file_info) == 0){
        // --- IDENTITY & STORAGE ---
        printf("[Storage Info]\n");
        printf("  Inode Number   : %ld\n", file_info.st_ino);
        printf("  Total Size     : %ld bytes\n", file_info.st_size);
        printf("  Disk Blocks    : %ld (512-byte blocks)\n", file_info.st_blocks);
        printf("  Hard Links     : %ld\n", file_info.st_nlink);

        // --- OWNERSHIP & PERMISSIONS ---
        printf("\n[Security Info]\n");
        printf("  Owner UID      : %d\n", file_info.st_uid);
        printf("  Group GID      : %d\n", file_info.st_gid);
        printf("  Permissions    : %o (Octal)\n", file_info.st_mode & 0777);

        // --- TIME (Using ctime to convert Epoch to Human-Readable) ---
        // NOTE: ctime() automatically adds a newline at the end of its string, 
        // so we DO NOT put \n in our printf!
        printf("\n[Time Info]\n");
        printf("  Last Accessed  : %s", ctime(&file_info.st_atime));
        printf("  Last Modified  : %s", ctime(&file_info.st_mtime));
        printf("  Status Changed : %s", ctime(&file_info.st_ctime));
    } 
    else {
        perror("Error: Could not get file stats");
    }

    return 0;
}