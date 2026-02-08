#include "shell.h"
#include <time.h>
/**
 * cmd_stat - Custom implementation of the 'stat' command.
 * It displays file size, inode, links, and access permissions.
 */
int cmd_stat(char **args) {
    if (args[1] == NULL) {
        printf("Usage: stat <filename>\n");
        return 1;
    }

    struct stat st;

    if (stat(args[1], &st) == -1) {
        perror("myshell stat");
        return 1;
    }

    printf("\033[1;36mFile: %s\033[0m\n", args[1]);
    printf("  Size: %-10ld  Blocks: %-10ld  IO Block: %-10ld ", 
            st.st_size, st.st_blocks, st.st_blksize);
    
    // Display File Type
    if (S_ISDIR(st.st_mode)) printf("directory\n");
    else if (S_ISREG(st.st_mode)) printf("regular file\n");
    else printf("special file\n");

    printf("  Device: %ldh/%ldd  Inode: %-10ld  Links: %ld\n", 
            st.st_dev, st.st_dev, st.st_ino, st.st_nlink);

    // Display Permissions in octal and rwxrwxrwx format
    printf("  Access: (%04o/", st.st_mode & 0777);
    printf((S_ISDIR(st.st_mode)) ? "d" : "-");
    printf((st.st_mode & S_IRUSR) ? "r" : "-");
    printf((st.st_mode & S_IWUSR) ? "w" : "-");
    printf((st.st_mode & S_IXUSR) ? "x" : "-");
    printf((st.st_mode & S_IRGRP) ? "r" : "-");
    printf((st.st_mode & S_IWGRP) ? "w" : "-");
    printf((st.st_mode & S_IXGRP) ? "x" : "-");
    printf((st.st_mode & S_IROTH) ? "r" : "-");
    printf((st.st_mode & S_IWOTH) ? "w" : "-");
    printf((st.st_mode & S_IXOTH) ? "x" : "-");
    printf(")  Uid: %d  Gid: %d\n", st.st_uid, st.st_gid);

    // Display Timestamps
    printf("  Modify: %s", ctime(&st.st_mtime));

    return 1;
}
