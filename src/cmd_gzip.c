#include "shell.h"

/**
 * cmd_gzip - Wrapper for gzip and gunzip utilities.
 * Handles:
 * - gzip <filename> (Compresses file to .gz)
 * - gunzip <filename.gz> (Decompresses .gz file)
 */
int cmd_gzip(char **args) {
    if (args[1] == NULL) {
        printf("%s: missing file operand\n", args[0]);
        printf("Usage: %s <filename>\n", args[0]);
        return 1;
    }

    /* Identify if the user called 'gzip' or 'gunzip' */
    char *command = args[0];

    pid_t pid = fork();
    if (pid == 0) {
        /* Child Process: Execute the system binary */
        if (execvp(command, args) == -1) {
            printf("myshell: %s command not found. Ensure gzip is installed.\n", command);
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork error");
    } else {
        /* Parent Process: Wait for the compression/extraction to finish */
        waitpid(pid, NULL, 0);
    }
    return 1;
}
