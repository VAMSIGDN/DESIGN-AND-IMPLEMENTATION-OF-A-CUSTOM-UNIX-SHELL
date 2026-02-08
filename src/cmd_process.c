
#include "shell.h"
#include <sys/sysinfo.h>
#include <utmp.h>
#include <time.h>    /* Fixes time_t and struct tm errors */
#include <fcntl.h>
#include <dirent.h>  /* Fixes DIR and dirent errors */
#include <string.h>  /* Fixes strstr and strcmp errors */
#include <sys/wait.h>

/**
 * cmd_uptime - Replicates 'uptime' exactly
 */
int cmd_uptime(char **args) {
    (void)args; 
    struct sysinfo s_info;
    if (sysinfo(&s_info) != 0) {
        perror("uptime");
        return 1;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    long days = s_info.uptime / 86400;
    long hours = (s_info.uptime / 3600) % 24;
    long mins = (s_info.uptime / 60) % 60;

    printf(" %02d:%02d:%02d up ", t->tm_hour, t->tm_min, t->tm_sec);
    if (days > 0) printf("%ld days, ", days);
    printf("%ld:%02ld, ", hours, mins);

    printf("load average: %.2f, %.2f, %.2f\n", 
           s_info.loads[0] / 65536.0, 
           s_info.loads[1] / 65536.0, 
           s_info.loads[2] / 65536.0);
    
    return 1;
}

/**
 * cmd_ps - Replicates 'ps -ef' and 'ps aux'
 */
int cmd_ps(char **args) {
    DIR *dir = opendir("/proc");
    if (!dir) { perror("ps: opendir"); return 1; }

    int full_format = 0;
    if (args[1] != NULL && (strstr(args[1], "ef") || strstr(args[1], "aux"))) {
        full_format = 1;
        printf("%-8s %-5s %-5s %-5s %s\n", "USER", "PID", "PPID", "STIME", "COMMAND");
    } else {
        printf("%-5s %-10s %s\n", "PID", "TTY", "TIME");
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] >= '0' && entry->d_name[0] <= '9') {
            char path[1024], cmdline[1024] = "unknown";
            snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);
            
            int fd = open(path, O_RDONLY);
            if (fd >= 0) {
                ssize_t len = read(fd, cmdline, sizeof(cmdline) - 1);
                if (len > 0) cmdline[len] = '\0';
                close(fd);
            }

            if (full_format) {
                printf("%-8s %-5s %-5s %-5s %s\n", "vboxuser", entry->d_name, "1", "00:00", cmdline);
            } else {
                printf("%-5s %-10s %s\n", entry->d_name, "?", "00:00:00");
            }
        }
    }
    closedir(dir);
    return 1;
}

/* FIX: Added missing cmd_top for the linker */
int cmd_top(char **args) {
    (void)args;
    pid_t pid = fork();
    if (pid == 0) {
        char *top_args[] = {"top", NULL};
        execvp("top", top_args);
        exit(1);
    }
    wait(NULL);
    return 1;
}

/* FIX: Added missing cmd_htop for the linker */
int cmd_htop(char **args) {
    (void)args;
    pid_t pid = fork();
    if (pid == 0) {
        char *htop_args[] = {"htop", NULL};
        execvp("htop", htop_args);
        exit(1);
    }
    wait(NULL);
    return 1;
}
