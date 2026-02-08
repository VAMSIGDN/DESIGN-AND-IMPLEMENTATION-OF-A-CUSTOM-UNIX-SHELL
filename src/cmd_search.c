#include "shell.h"
#include <sys/stat.h>

// Helper to check if a file exists and is executable
int is_executable(char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && (st.st_mode & S_IXUSR));
}

int cmd_search(char **args) {
    if (args[1] == NULL) {
        printf("Usage: %s <command_name>\n", args[0]);
        return 1;
    }

    char *path_env = getenv("PATH");
    if (!path_env) {
        fprintf(stderr, "myshell: PATH environment variable not found\n");
        return 1;
    }

    // Duplicate PATH because strtok modifies the string
    char *path_dup = strdup(path_env);
    char *dir = strtok(path_dup, ":");
    int found = 0;

    printf("\033[1;33mSearching for: %s\033[0m\n", args[1]);

    while (dir != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, args[1]);

        if (is_executable(full_path)) {
            if (strcmp(args[0], "which") == 0) {
                printf("%s\n", full_path);
                found = 1;
                break; // 'which' usually stops at the first match
            } else {
                // 'whereis' shows all locations and potentially source/man paths
                printf("%s: %s\n", args[1], full_path);
                found = 1;
            }
        }
        dir = strtok(NULL, ":");
    }

    if (!found) {
        printf("%s not found in PATH\n", args[1]);
    }

    free(path_dup);
    return 1;
}
