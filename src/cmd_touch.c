#include "shell.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <utime.h>

/**
 * cmd_touch - Creates empty files or updates access/modification times.
 * Implementation for all standard touch use cases.
 */
int cmd_touch(char **args) {
    if (args[1] == NULL) {
        printf("touch: missing file operand\n");
        return 1;
    }

    // Loop through all provided filenames
    for (int i = 1; args[i] != NULL; i++) {
        /* Open with O_CREAT: Create if doesn't exist.
           Permission 0644: rw-r--r-- */
        int fd = open(args[i], O_WRONLY | O_CREAT, 0644);
        
        if (fd == -1) {
            perror("touch");
            continue;
        }
        
        /* Update timestamps to the current system time using utime.
           Passing NULL as the second argument sets it to 'now'. */
        if (utime(args[i], NULL) == -1) {
            perror("touch: utime error");
        }
        
        close(fd);
    }
    return 1;
}
