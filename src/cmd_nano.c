#include "shell.h"

/**
 * cmd_nano - Launches the Nano editor for a specified file.
 */
int cmd_nano(char **args) {
    if (args[1] == NULL) {
        printf("nano: missing file operand\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child Process: Hand over control to nano
        if (execvp("nano", args) == -1) {
            printf("myshell: nano command not found. Please install it.\n");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork error");
    } else {
        // Parent Process: Wait until the user finishes editing
        waitpid(pid, NULL, 0);
    }
    return 1;
}
