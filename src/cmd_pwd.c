#include "shell.h"
#include <limits.h>

/**
 * cmd_pwd - Handles the 'pwd' command.
 * Supports:
 * - pwd (default logical)
 * - pwd -L (Logical path from environment)
 * - pwd -P (Physical path from system)
 */
int cmd_pwd(char **args) {
    char cwd[PATH_MAX];
    int physical = 0;

    // Parse flags
    if (args[1] != NULL) {
        if (strcmp(args[1], "-P") == 0) {
            physical = 1;
        } else if (strcmp(args[1], "-L") == 0) {
            physical = 0;
        }
    }

    if (physical) {
        // Get actual physical path from the system
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("myshell pwd -P");
        }
    } else {
        // Get logical path from environment (standard shell behavior)
        char *logical_pwd = getenv("PWD");
        if (logical_pwd != NULL) {
            printf("%s\n", logical_pwd);
        } else {
            // Fallback to getcwd if environment is missing
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        }
    }

    return 1;
}
