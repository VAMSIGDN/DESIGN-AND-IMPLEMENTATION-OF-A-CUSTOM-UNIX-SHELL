#include "shell.h"
#include <sys/stat.h>
#include <errno.h>

/**
 * mkdir_recursive - Logic for the -p flag
 * Iteratively creates parent directories if they don't exist.
 */
int mkdir_recursive(const char *path, mode_t mode, int verbose) {
    char tmp[1024];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') tmp[len - 1] = 0;

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            if (mkdir(tmp, mode) == 0 && verbose) {
                printf("mkdir: created directory '%s'\n", tmp);
            }
            *p = '/';
        }
    }
    if (mkdir(tmp, mode) == 0) {
        if (verbose) printf("mkdir: created directory '%s'\n", tmp);
        return 0;
    }
    return (errno == EEXIST) ? 0 : -1;
}

int cmd_mkdir(char **args) {
    int p_flag = 0, v_flag = 0;
    int i = 1;

    // Parse flags
    while (args[i] && args[i][0] == '-') {
        if (strchr(args[i], 'p')) p_flag = 1;
        if (strchr(args[i], 'v')) v_flag = 1;
        i++;
    }

    if (!args[i]) {
        printf("mkdir: missing operand\n");
        return 1;
    }

    while (args[i]) {
        if (p_flag) {
            if (mkdir_recursive(args[i], 0755, v_flag) != 0 && errno != EEXIST) {
                perror("mkdir");
            }
        } else {
            if (mkdir(args[i], 0755) == -1) {
                perror("mkdir");
            } else if (v_flag) {
                printf("mkdir: created directory '%s'\n", args[i]);
            }
        }
        i++;
    }
    return 1;
}

int cmd_rmdir(char **args) {
    int p_flag = 0;
    int i = 1;

    if (args[i] && strcmp(args[i], "-p") == 0) {
        p_flag = 1;
        i++;
    }

    if (!args[i]) {
        printf("rmdir: missing operand\n");
        return 1;
    }

    while (args[i]) {
        if (p_flag) {
            char *path = strdup(args[i]);
            while (strlen(path) > 0) {
                if (rmdir(path) == -1) {
                    if (errno != ENOTEMPTY) perror("rmdir");
                    break;
                }
                char *slash = strrchr(path, '/');
                if (!slash) break;
                *slash = '\0';
            }
            free(path);
        } else {
            if (rmdir(args[i]) == -1) perror("rmdir");
        }
        i++;
    }
    return 1;
}
