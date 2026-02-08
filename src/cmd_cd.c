#include "shell.h"
#include <limits.h> // For PATH_MAX

int cmd_cd(char **args) {
    char cwd[PATH_MAX];
    char *target = NULL;

    // Get current directory before changing (to update OLDPWD later)
    getcwd(cwd, sizeof(cwd));

    // Case 1: "cd" or "cd ~" -> Go to Home Directory
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
        target = getenv("HOME");
        if (target == NULL) {
            fprintf(stderr, "myshell: HOME not set\n");
            return 1;
        }
    } 
    // Case 2: "cd -" -> Go to Previous Working Directory
    else if (strcmp(args[1], "-") == 0) {
        target = getenv("OLDPWD");
        if (target == NULL) {
            fprintf(stderr, "myshell: OLDPWD not set\n");
            return 1;
        }
        printf("%s\n", target); // Standard shells print the path for 'cd -'
    } 
    // Case 3: "cd .." or "cd path/to/dir"
    else {
        target = args[1];
    }

    // Perform the actual directory change
    if (chdir(target) != 0) {
        perror("myshell cd");
    } else {
        // Update Environment Variables for the shell state
        setenv("OLDPWD", cwd, 1); // Set old path
        getcwd(cwd, sizeof(cwd));
        setenv("PWD", cwd, 1);    // Set new path
    }

    return 1;
}

