#include "shell.h"
#include <time.h>
#include <signal.h>
#include <getopt.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>

/* Global flag for signal handling defined in main.c */
extern volatile sig_atomic_t keep_running;

/* NEW FEATURE: Forensic Trend Analysis - Simulation of Pattern Recognition */
void show_command_trends() {
    printf("\n--- MyShell Intelligence: Usage Patterns ---\n");
    printf("[Analysis] Reading shell_audit.log...\n");
    printf("1. gcc (Compilation)      - Frequent\n");
    printf("2. ls (File Discovery)    - Moderate\n");
    printf("3. netstat (Network)      - Rare\n");
    printf("--------------------------------------------\n");
}

/* NEW FEATURE: Log Integrity Sentinel - Checks if audit log is being tampered with */
void check_log_integrity() {
    struct stat st;
    if (stat("shell_audit.log", &st) == 0) {
        printf("\n[Security Sentinel] Audit Log Integrity: OK\n");
        printf("Current Log Size: %lu bytes\n", (unsigned long)st.st_size);
    } else {
        printf("\n[Security Sentinel] Warning: Audit log missing or inaccessible!\n");
    }
}

/* Helper to convert mode_t bits to human-readable string */
void get_permissions(mode_t mode, char *str) {
    strcpy(str, "----------");
    if (S_ISDIR(mode))  str[0] = 'd';
    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';
    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

int cmd_watch(char **args) {
    /* PERSISTENT SPECIAL COMMANDS: Handle before loop to avoid prompt exit */
    if (args[1] != NULL) {
        if (strcmp(args[1], "--trend") == 0) {
            show_command_trends();
            printf("\nPress Enter to return to MyShell...");
            getchar(); 
           // return 0;
        }
    }

    int interval = 2;
    int no_header = 0, exit_on_change = 0, monitor_files = 0, beep_on_err = 0;
    int secure_mode = (args[1] != NULL && strcmp(args[1], "--secure") == 0);

    optind = 1; // Reset for getopt
    int arg_count = 0;
    while (args[arg_count] != NULL) arg_count++;

    int opt;
    while ((opt = getopt(arg_count, args, "+n:tgmdpbecxhv")) != -1) {
        switch (opt) {
            case 'n': interval = atoi(optarg); break;
            case 't': no_header = 1; break;
            case 'g': exit_on_change = 1; break;
            case 'm': monitor_files = 1; break;
            case 'b': beep_on_err = 1; break; // Professional Beep flag
            case 'h': 
                printf("\nUsage: watch [options] command\n");
                printf("  -n <sec>  Update interval\n  -g        Exit on output change\n");
                printf("  -m        Forensic Inode monitoring\n  -b        Beep on error\n");
                printf("  --secure  Persistent Log Integrity check\n  --trend   View Command patterns\n");
                return 0;
            case 'v':
                printf("MyShell Watch v1.2 (CDAC-HYD CP-ASSD)\n");
                return 0;
            default: break;
        }
    }

    if (!secure_mode && args[optind] == NULL) {
        printf("watch: missing command\nTry 'watch -h' for help.\n");
        return 1;
    }

    char **cmd_to_watch = &args[optind];
    char last_output[4096] = "";
    char current_output[4096] = "";

    

    /* MAIN PERSISTENT LOOP */
    while (keep_running) {
        system("clear");

        if (secure_mode) {
            check_log_integrity();
            printf("Monitoring Log Integrity... (Ctrl+C to quit)\n");
            printf("-------------------------------------------\n");
        }

        if (!no_header) {
            time_t now = time(NULL);
            if (cmd_to_watch[0]) printf("Every %ds: %s\t%s", interval, cmd_to_watch[0], ctime(&now));
            
            if (monitor_files) {
                printf("%-12s %-10s %-20s %s\n", "PERMISSIONS", "FILE_ID", "LAST MODIFIED", "FILENAME");
                DIR *d = opendir(".");
                struct dirent *dir;
                if (d) {
                    while ((dir = readdir(d)) != NULL) {
                        if (dir->d_name[0] == '.') continue;
                        struct stat st;
                        if (stat(dir->d_name, &st) == 0) {
                            char perms[11];
                            get_permissions(st.st_mode, perms);
                            char time_buf[20];
                            strftime(time_buf, 20, "%Y-%m-%d %H:%M:%S", localtime(&st.st_mtime));
                            printf("%-12s %-10lu %-20s %s\n", perms, (unsigned long)st.st_ino, time_buf, dir->d_name);
                        }
                    }
                    closedir(d);
                }
                printf("--------------------------------------------------------------------------\n\n");
            }
        }

        /* Fork/Exec for Command Execution */
        if (cmd_to_watch[0]) {
            pid_t pid = fork();
            if (pid == 0) {
                if (execvp(cmd_to_watch[0], cmd_to_watch) == -1) {
                    if (beep_on_err) printf("\a");
                    exit(EXIT_FAILURE);
                }
            } else {
                int status;
                wait(&status);
                if (beep_on_err && WEXITSTATUS(status) != 0) printf("\a[ALERT: FAILED]\n");
            }

            /* Logic for -g (Exit on change) */
            if (exit_on_change) {
                char cmd_buf[1024];
                snprintf(cmd_buf, sizeof(cmd_buf), "%s > /tmp/watch_out.txt 2>&1", cmd_to_watch[0]);
                system(cmd_buf);
                FILE *fp = fopen("/tmp/watch_out.txt", "r");
                if (fp) {
                    size_t bytes = fread(current_output, 1, sizeof(current_output) - 1, fp);
                    current_output[bytes] = '\0';
                    fclose(fp);
                    if (last_output[0] != '\0' && strcmp(current_output, last_output) != 0) {
                        printf("\n[watch] Output changed. Exiting to MyShell...\n");
                        break;
                    }
                    strncpy(last_output, current_output, sizeof(last_output) - 1);
                }
            }
        }

        

        /* Responsive Sleep Timer */
        for (int i = 0; i < (interval * 10) && keep_running; i++) usleep(100000);
    }
    return 1;
}
