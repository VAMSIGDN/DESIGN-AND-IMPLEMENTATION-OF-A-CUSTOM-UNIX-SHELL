#include "shell.h"

/**
 * Storage Management Implementation
 * This module wraps standard Linux utilities to provide a 
 * native shell experience for disk management.
 */

int cmd_df(char **args) {
    // If user just types 'df', we helpfully add '-h'
    if (args[1] == NULL) {
        char *default_args[] = {"df", "-h", NULL};
        return execvp("df", default_args);
    }
    return execvp("df", args);
}

int cmd_du(char **args) {
    // Default to summary and human-readable for the current directory
    if (args[1] == NULL) {
        char *default_args[] = {"du", "-sh", ".", NULL};
        return execvp("du", default_args);
    }
    return execvp("du", args);
}

int cmd_lsblk(char **args) {
    return execvp("lsblk", args);
}

int cmd_blkid(char **args) {
    // blkid usually requires root privileges to see all attributes
    if (getuid() != 0) {
        printf("myshell: blkid: permission denied (try with sudo)\n");
        return 1;
    }
    return execvp("blkid", args);
}

int cmd_mount(char **args) {
    if (args[1] == NULL) {
        // Just 'mount' lists current mount points
        return execvp("mount", args);
    }
    // Security check: Ensure only root can mount devices
    if (getuid() != 0) {
        printf("myshell: mount: root privileges required\n");
        return 1;
    }
    return execvp("mount", args);
}

int cmd_umount(char **args) {
    if (getuid() != 0) {
        printf("myshell: umount: root privileges required\n");
        return 1;
    }
    return execvp("umount", args);
}
