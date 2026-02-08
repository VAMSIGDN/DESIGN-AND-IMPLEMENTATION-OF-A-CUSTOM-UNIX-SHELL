#include "shell.h"

/**
 * cmd_tar - Wrapper for the tar utility.
 * Handles:
 * -cvf : Create archive
 * -xvf : Extract archive
 * -czvf: Create gzipped archive
 * -xvzf: Extract gzipped archive
 */
int cmd_tar(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("tar: usage: tar <flags> <archive_name> <files...>\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child Process: Hand over execution to the system's tar binary
        if (execvp("tar", args) == -1) {
            printf("myshell: tar command not found on this system.\n");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork error");
    } else {
        // Parent Process: Wait for the compression/extraction to finish
        waitpid(pid, NULL, 0);
    }
    return 1;
}
