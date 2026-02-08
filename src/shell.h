#ifndef SHELL_H
#define SHELL_H

/* --- Constants --- */
#define MAX_LINE 1024
#define MAX_HABITS 10
#define TOK_DELIM " \t\r\n\a"
#define COLOR_PROMPT "\033[1;32m"
#define COLOR_RESET  "\033[0m"
#define COLOR_INFO "\033[1;34m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <utime.h>
#include <dirent.h>

extern volatile sig_atomic_t keep_running;

/* --- Data Structures --- */
typedef struct {
    char command[MAX_LINE];
    int count;
} CommandHabit;

/* --- Core Engine --- */
char *read_line(void);
char **parse_line(char *line);
char **parse_input_with_alias(char *line);
int execute_command(char **args);
void handle_sigint(int sig);

/* --- Feature Prototypes --- */
char *get_alias_value(char *key);     // Logic in cmd_alias.c
void analyze_and_suggest(char *cmd); // Logic in cmd_suggest.c

/* --- Command Prototypes --- */
int cmd_ls(char **args);
int cmd_cd(char **args);
int cmd_pwd(char **args);
int cmd_cat(char **args);
int cmd_mkdir(char **args);
int cmd_rmdir(char **args);
int cmd_cp(char **args);
int cmd_mv(char **args);
int cmd_rm(char **args);
int cmd_touch(char **args);
int cmd_nano(char **args);
int cmd_vi(char **args);
int cmd_tar(char **args);
int cmd_zip(char **args);
int cmd_gzip(char **args);
int cmd_stat(char **args);
int cmd_exit(char **args);
int cmd_help(char **args);
int cmd_search(char **args);
int cmd_pager(char **args);
int cmd_man(char **args);
int cmd_file(char **args);
int cmd_text_utils(char **args);
int cmd_ps(char **args);
int cmd_top(char **args);
int cmd_htop(char **args);
int cmd_uptime(char **args);
int cmd_watch(char **args);
int cmd_kill(char **args);
int cmd_pkill(char **args);
int cmd_killall(char **args);
int cmd_net(char **args);
int cmd_alias(char **args);
int cmd_unalias(char **args);
/* Storage Suite (Implementation in cmd_storage.c) */
int cmd_df(char **args);
int cmd_du(char **args);
int cmd_lsblk(char **args);
int cmd_blkid(char **args);
int cmd_mount(char **args);
int cmd_umount(char **args);

#endif
