#include "shell.h"
#include <sys/stat.h>

/**
 * cmd_mv - Custom implementation of the 'mv' command.
 * Logic:
 * 1. Handle -i (interactive), -v (verbose), and -u (update).
 * 2. Utilize rename() for efficient move operations.
 */
int cmd_mv(char **args) {
    int i_flag = 0, v_flag = 0, u_flag = 0;
    int idx = 1;

    // Flag parsing
    while (args[idx] && args[idx][0] == '-') {
        if (strchr(args[idx], 'i')) i_flag = 1;
        if (strchr(args[idx], 'v')) v_flag = 1;
        if (strchr(args[idx], 'u')) u_flag = 1;
        idx++;
    }

    // Check if both source and destination are provided
    if (!args[idx] || !args[idx + 1]) {
        printf("mv: missing file operand\n");
        return 1;
    }

    char *src = args[idx];
    char *dest = args[idx + 1];
    struct stat st_src, st_dest;

    if (stat(src, &st_src) != 0) {
        perror("mv: source");
        return 1;
    }

    // Handle -u: Move only if source is newer than destination
    if (u_flag && stat(dest, &st_dest) == 0) {
        if (st_src.st_mtime <= st_dest.st_mtime) return 1;
    }

    // Handle -i: Interactive prompt if destination exists
    if (i_flag && access(dest, F_OK) == 0) {
        printf("mv: overwrite '%s'? (y/n): ", dest);
        char choice = getchar();
        while (getchar() != '\n'); // clear buffer
        if (choice != 'y' && choice != 'Y') return 1;
    }

    // Attempt to move/rename
    if (rename(src, dest) == 0) {
        if (v_flag) printf("renamed '%s' -> '%s'\n", src, dest);
    } else {
        perror("mv error");
    }

    return 1;
}
