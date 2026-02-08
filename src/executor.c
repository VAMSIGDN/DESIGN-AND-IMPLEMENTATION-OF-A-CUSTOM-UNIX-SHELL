#include "shell.h"
#include <time.h>
int execute_command(char **args) {
    if (args[0] == NULL) return 1;
    
    // --- SECURE AUDIT LOGGER WITH TIMESTAMP ---
    FILE *log_file = fopen("shell_audit.log", "a");
    if (log_file != NULL) {
        time_t now = time(NULL);
        char *timestamp = ctime(&now);
        timestamp[strlen(timestamp) - 1] = '\0';

        fprintf(log_file, "[%s] [AUDIT] User %d executed: ", timestamp, getuid());
        for (int i = 0; args[i] != NULL; i++) {
            fprintf(log_file, "%s ", args[i]);
        }
        fprintf(log_file, "\n");
        fclose(log_file);
    }

    // Shell Navigation
    if (strcmp(args[0], "cd") == 0)      return cmd_cd(args);
    if (strcmp(args[0], "pwd") == 0)     return cmd_pwd(args);
    if (strcmp(args[0], "exit") == 0)    return cmd_exit(args);
    if (strcmp(args[0], "help") == 0)    return cmd_help(args);

    // File Management
    if (strcmp(args[0], "ls") == 0)      return cmd_ls(args);
    if (strcmp(args[0], "mkdir") == 0)   return cmd_mkdir(args);
    if (strcmp(args[0], "rmdir") == 0)   return cmd_rmdir(args);
    if (strcmp(args[0], "cp") == 0)      return cmd_cp(args);
    if (strcmp(args[0], "mv") == 0)      return cmd_mv(args);
    if (strcmp(args[0], "rm") == 0)      return cmd_rm(args);
    if (strcmp(args[0], "touch") == 0)   return cmd_touch(args);
    if (strcmp(args[0], "stat") == 0)    return cmd_stat(args);
    if (strcmp(args[0], "file") == 0)    return cmd_file(args);

    // Editing & View
    if (strcmp(args[0], "cat") == 0)     return cmd_cat(args);
    if (strcmp(args[0], "more") == 0)    return cmd_pager(args);
    if (strcmp(args[0], "nano") == 0)    return cmd_nano(args);
    if (strcmp(args[0], "vi") == 0)      return cmd_vi(args);
    
    // Process Control
    if (strcmp(args[0], "kill") == 0)    return cmd_kill(args);
    if (strcmp(args[0], "pkill") == 0)   return cmd_pkill(args);
    if (strcmp(args[0], "killall") == 0) return cmd_killall(args);
    if (strcmp(args[0], "ps") == 0)      return cmd_ps(args);
    if (strcmp(args[0], "top") == 0)      return cmd_top(args);
    if (strcmp(args[0], "htop") == 0)     return cmd_htop(args);
    if (strcmp(args[0], "uptime") == 0)   return cmd_uptime(args);
    if (strcmp(args[0], "watch") == 0) {
        int i = 1, j = 1;
        while (args[i] != NULL) {
            if (strcmp(args[i], "-d") == 0 || strcmp(args[i], "-e") == 0 || 
                strcmp(args[i], "-b") == 0) {
                i++;
                continue;
            }
            args[j++] = args[i++];
        }
        args[j] = NULL;
        return cmd_watch(args);
    }

    // Storage Management (Mapped to cmd_storage.c)
    if (strcmp(args[0], "df") == 0)      return cmd_df(args);
    if (strcmp(args[0], "du") == 0)      return cmd_du(args);
    if (strcmp(args[0], "lsblk") == 0)   return cmd_lsblk(args);
    if (strcmp(args[0], "blkid") == 0)   return cmd_blkid(args);
    if (strcmp(args[0], "mount") == 0)   return cmd_mount(args);
    if (strcmp(args[0], "umount") == 0)  return cmd_umount(args);

    // Archive & Compression
    if (strcmp(args[0], "tar") == 0)      return cmd_tar(args);
    if (strcmp(args[0], "zip") == 0)      return cmd_zip(args);
    if (strcmp(args[0], "unzip") == 0)    return cmd_zip(args);
    if (strcmp(args[0], "gzip") == 0)     return cmd_gzip(args);
    if (strcmp(args[0], "gunzip") == 0)   return cmd_gzip(args);

    // Alias Management
    if (strcmp(args[0], "alias") == 0)    return cmd_alias(args);
    if (strcmp(args[0], "unalias") == 0)  return cmd_unalias(args);

    // Networking Suite
    if (strcmp(args[0], "ip") == 0 || strcmp(args[0], "ping") == 0 ||
        strcmp(args[0], "ss") == 0 || strcmp(args[0], "dig") == 0 ||
        strcmp(args[0], "traceroute") == 0 || strcmp(args[0], "nmcli") == 0) {
        return cmd_net(args);
    }

    /* --- System Fallback with Security & Audit --- */
    
    // 1. CP-ASSD Security Alert: Intercept sensitive system commands
    if (strcmp(args[0], "fdisk") == 0 || strcmp(args[0], "mkfs") == 0) {
        if (getuid() != 0) {
            printf("\033[1;31m[Security Alert]\033[0m '%s' requires root privileges.\n", args[0]);
            return 1;
        }
    }

    // 3. System Execution
    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            printf("myshell: command not found: %s\n", args[0]);
	    exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("myshell: fork failed");
    } else {
        waitpid(pid, NULL, 0);
    }
    return 1;
}
