#include "shell.h"

/**
 * cmd_zip - Wrapper for zip and unzip utilities.
 * Handles:
 * - zip <archive_name> <files...>
 * - unzip <archive_name>
 */
int cmd_zip(char **args) {
    if (args[1] == NULL) {
        printf("zip/unzip: missing operands\n");
        printf("Usage: zip <archive.zip> <files...> OR unzip <archive.zip>\n");
        return 1;
    }

    char *command = args[0]; // Either "zip" or "unzip"

    pid_t pid = fork();
    if (pid == 0) {
        // Child Process: Hand over to the system binary
        if (execvp(command, args) == -1) {
            printf("myshell: %s command not found. Please install zip/unzip.\n", command);
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork error");
    } else {
        // Parent Process: Wait for compression/decompression to finish
        waitpid(pid, NULL, 0);
    }
    return 1;
}
