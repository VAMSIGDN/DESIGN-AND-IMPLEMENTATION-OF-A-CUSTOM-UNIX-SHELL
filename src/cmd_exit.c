#include "shell.h"

int cmd_exit(char **args) {
    int status = 0;
    if (args[1] != NULL) {
        status = atoi(args[1]);
    }

    /* Fixed: Added placeholders for colors and status */
    printf("%sExiting myshell... [Status %d]%s\n", COLOR_INFO, status, COLOR_RESET);
    
    exit(status);
    return 0;
}
