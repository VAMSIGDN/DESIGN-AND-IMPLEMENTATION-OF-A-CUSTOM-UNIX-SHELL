#include "shell.h"

/**
 * cmd_man - Custom manual page reader.
 * Logic: 
 * 1. If it's a custom command (ls, cd, etc.), show your built-in manual.
 * 2. If it's a system command, use fork-exec to call the real 'man'.
 */
int cmd_man(char **args) {
    if (args[1] == NULL) {
        printf("What manual page do you want?\nExample: man ls\n");
        return 1;
    }

    // Check for custom commands first to show your project's logic
    if (strcmp(args[1], "ls") == 0) {
        printf("\033[1;36mMANUAL: CUSTOM LS\033[0m\n");
        printf("USAGE: ls [-a] [-l]\n");
        printf("DESCRIPTION: Lists directory contents with color coding.\n");
        printf("  -a : Show hidden files.\n  -l : Use long listing format.\n");
        return 1;
    } 
    else if (strcmp(args[1], "cd") == 0) {
        printf("\033[1;36mMANUAL: CUSTOM CD\033[0m\n");
        printf("USAGE: cd [path | ~ | -]\n");
        printf("DESCRIPTION: Changes the current working directory.\n");
        printf("  ~ : Home directory.\n  - : Previous directory.\n");
        return 1;
    }

    // If not a custom command, delegate to the system manual
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp("man", args) == -1) {
            perror("myshell man");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork error");
    } else {
        // Parent waits for the user to finish reading the system manual
        waitpid(pid, NULL, 0);
    }

    return 1;
}
