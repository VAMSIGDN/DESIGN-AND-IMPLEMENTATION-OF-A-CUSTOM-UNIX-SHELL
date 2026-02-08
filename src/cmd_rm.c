#include "shell.h"
#include <sys/stat.h>
#include <dirent.h>

/**
 * remove_recursive - Logic for the -r flag
 * Traverses directories and deletes content before the directory itself.
 */
int remove_recursive(const char *path, int i_flag) {
    struct stat st;
    if (stat(path, &st) != 0) return -1;

    if (S_ISDIR(st.st_mode)) {
        DIR *d = opendir(path);
        if (!d) return -1;

        struct dirent *p;
        while ((p = readdir(d))) {
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) continue;

            char buf[1024];
            snprintf(buf, sizeof(buf), "%s/%s", path, p->d_name);
            remove_recursive(buf, i_flag);
        }
        closedir(d);
        
        if (i_flag) {
            printf("rm: remove directory '%s'? (y/n): ", path);
            char c = getchar(); while(getchar() != '\n');
            if (c != 'y' && c != 'Y') return 0;
        }
        return rmdir(path);
    }

    if (i_flag) {
        printf("rm: remove regular file '%s'? (y/n): ", path);
        char c = getchar(); while(getchar() != '\n');
        if (c != 'y' && c != 'Y') return 0;
    }
    return unlink(path);
}

int cmd_rm(char **args) {
    int i_flag = 0, r_flag = 0;
    int idx = 1;

    // Parse flags: -i (interactive), -r (recursive)
    while (args[idx] && args[idx][0] == '-') {
        if (strchr(args[idx], 'i')) i_flag = 1;
        if (strchr(args[idx], 'r')) r_flag = 1;
        idx++;
    }

    if (!args[idx]) {
        printf("rm: missing operand\n");
        return 1;
    }

    while (args[idx]) {
        if (r_flag) {
            if (remove_recursive(args[idx], i_flag) != 0) perror("rm");
        } else {
            struct stat st;
            if (stat(args[idx], &st) == 0 && S_ISDIR(st.st_mode)) {
                printf("rm: cannot remove '%s': Is a directory\n", args[idx]);
            } else {
                if (i_flag) {
                    printf("rm: remove '%s'? (y/n): ", args[idx]);
                    char c = getchar(); while(getchar() != '\n');
                    if (c == 'y' || c == 'Y') unlink(args[idx]);
                } else {
                    if (unlink(args[idx]) != 0) perror("rm");
                }
            }
        }
        idx++;
    }
    return 1;
}
