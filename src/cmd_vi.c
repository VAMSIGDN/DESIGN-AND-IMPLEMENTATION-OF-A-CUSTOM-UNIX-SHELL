#include "shell.h"

/**
 * cmd_vi - Launches the Vi editor for a specified file.
 */
int cmd_vi(char **args) {
    if (args[1] == NULL) {
        printf("vi: missing file operand\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child Process: Execute vi
        if (execvp("vi", args) == -1) {
            printf("myshell: vi command not found.\n");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork error");
    } else {
        // Parent Process: Pause shell until vi is closed
        waitpid(pid, NULL, 0);
    }
    return 1;
}
