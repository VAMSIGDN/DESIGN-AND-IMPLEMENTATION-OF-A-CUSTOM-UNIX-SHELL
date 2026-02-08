#include "shell.h"
#include <signal.h>
#include <dirent.h>
#include <ctype.h>

/**
 * cmd_kill - Replicates 'kill' command
 * Usage: kill <pid> or kill -<signal> <pid>
 */
int cmd_kill(char **args) {
    if (args[1] == NULL) {
        printf("kill: usage: kill [-signal] <pid>\n");
        return 1;
    }

    int sig = SIGTERM; // Default -15
    char *pid_str = args[1];

    if (args[1][0] == '-') {
        sig = atoi(&args[1][1]);
        pid_str = args[2];
    }

    if (pid_str == NULL) {
        printf("kill: pid required\n");
        return 1;
    }

    if (kill(atoi(pid_str), sig) == -1) {
        perror("kill");
    }
    return 1;
}

/**
 * cmd_killall_pkill - Logic for killall and pkill
 * Matches process names and sends signals to user-owned processes.
 */
int handle_pattern_kill(char *pattern, int sig) {
    DIR *dir = opendir("/proc");
    if (!dir) return 1;

    uid_t current_uid = getuid();
    struct dirent *entry;

    while ((entry = readdir(dir))) {
        if (isdigit(entry->d_name[0])) {
            char path[1024], cmdline[1024];
            struct stat st;
            snprintf(path, sizeof(path), "/proc/%s", entry->d_name);
            
            if (stat(path, &st) == 0 && st.st_uid == current_uid) {
                snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);
                FILE *f = fopen(path, "r");
                if (f) {
                    if (fgets(cmdline, sizeof(cmdline), f)) {
                        cmdline[strcspn(cmdline, "\n")] = 0;
                        if (strstr(cmdline, pattern)) {
                            kill(atoi(entry->d_name), sig);
                        }
                    }
                    fclose(f);
                }
            }
        }
    }
    closedir(dir);
    return 1;
}

int cmd_pkill(char **args) {
    if (args[1] == NULL) {
        printf("pkill: pattern required\n");
        return 1;
    }
    return handle_pattern_kill(args[1], SIGTERM);
}

int cmd_killall(char **args) {
    if (args[1] == NULL) {
        printf("killall: process name required\n");
        return 1;
    }
    return handle_pattern_kill(args[1], SIGTERM);
}
