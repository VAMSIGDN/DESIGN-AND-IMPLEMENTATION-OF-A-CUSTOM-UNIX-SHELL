#include "shell.h"
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

/* Global state for scandir sorting */
static int g_sort_time = 0;
static int g_sort_size = 0;
static int g_sort_reverse = 0;
static char g_current_dir[1024];

void print_human_size(long size) {
    const char *units[] = {"B", "K", "M", "G", "T"};
    int i = 0;
    double d_size = (double)size;
    while (d_size > 1024 && i < 4) {
        d_size /= 1024;
        i++;
    }
    if (i == 0) printf(" %5ld", size);
    else printf(" %4.1f%s", d_size, units[i]);
}

void print_long_format(char *name, struct stat *f, int h_flag, int color) {
    printf((S_ISDIR(f->st_mode)) ? "d" : (S_ISLNK(f->st_mode)) ? "l" : "-");
    printf((f->st_mode & S_IRUSR) ? "r" : "-");
    printf((f->st_mode & S_IWUSR) ? "w" : "-");
    printf((f->st_mode & S_IXUSR) ? "x" : "-");
    printf((f->st_mode & S_IRGRP) ? "r" : "-");
    printf((f->st_mode & S_IWGRP) ? "w" : "-");
    printf((f->st_mode & S_IXGRP) ? "x" : "-");
    printf((f->st_mode & S_IROTH) ? "r" : "-");
    printf((f->st_mode & S_IWOTH) ? "w" : "-");
    printf((f->st_mode & S_IXOTH) ? "x" : "-");

    printf(" %2ld", f->st_nlink);
    struct passwd *pw = getpwuid(f->st_uid);
    struct group *gr = getgrgid(f->st_gid);
    printf(" %s %s", pw ? pw->pw_name : "unknown", gr ? gr->gr_name : "unknown");

    if (h_flag) print_human_size(f->st_size);
    else printf(" %8ld", f->st_size);

    char timeStr[20];
    strftime(timeStr, 20, "%b %d %H:%M", localtime(&f->st_mtime));
    printf(" %s ", timeStr);

    if (color) {
        if (S_ISDIR(f->st_mode)) printf("\033[1;34m%s\033[0m\n", name);
        else if (f->st_mode & S_IXUSR) printf("\033[1;35m%s\033[0m\n", name);
        else printf("%s\n", name);
    } else printf("%s\n", name);
}

int compare_entries(const struct dirent **a, const struct dirent **b) {
    struct stat st_a, st_b;
    char path_a[2048], path_b[2048];
    snprintf(path_a, sizeof(path_a), "%s/%s", g_current_dir, (*a)->d_name);
    snprintf(path_b, sizeof(path_b), "%s/%s", g_current_dir, (*b)->d_name);
    stat(path_a, &st_a);
    stat(path_b, &st_b);

    int result = 0;
    if (g_sort_time) result = (st_b.st_mtime - st_a.st_mtime);
    else if (g_sort_size) result = (st_b.st_size - st_a.st_size);
    else result = strcmp((*a)->d_name, (*b)->d_name);

    return g_sort_reverse ? -result : result;
}

void list_dir(const char *target, int a, int l, int h, int t, int s, int r, int R, int color) {
    g_sort_time = t; g_sort_size = s; g_sort_reverse = r;
    strncpy(g_current_dir, target, 1024);

    struct dirent **namelist;
    int n = scandir(target, &namelist, NULL, compare_entries);
    if (n < 0) { perror("scandir"); return; }

    if (l) {
        long total_blocks = 0;
        for (int i = 0; i < n; i++) {
            if (!a && namelist[i]->d_name[0] == '.') continue;
            struct stat st;
            char p[2048];
            snprintf(p, 2048, "%s/%s", target, namelist[i]->d_name);
            if (stat(p, &st) == 0) total_blocks += st.st_blocks;
        }
        printf("total %ld\n", total_blocks / 2);
    }

    for (int i = 0; i < n; i++) {
        if (!a && namelist[i]->d_name[0] == '.') continue;
        char path[2048];
        struct stat fStat;
        snprintf(path, 2048, "%s/%s", target, namelist[i]->d_name);
        stat(path, &fStat);

        if (l) print_long_format(namelist[i]->d_name, &fStat, h, color);
        else {
            if (color) {
                if (S_ISDIR(fStat.st_mode)) printf("\033[1;34m%s\033[0m  ", namelist[i]->d_name);
                else if (fStat.st_mode & S_IXUSR) printf("\033[1;35m%s\033[0m  ", namelist[i]->d_name);
                else printf("%s  ", namelist[i]->d_name);
            } else printf("%s  ", namelist[i]->d_name);
        }
        free(namelist[i]);
    }
    if (!l) printf("\n");
    free(namelist);

    if (R) { /* Recursive implementation here */ }
}

int cmd_ls(char **args) {
    int a=0, l=0, h=0, t=0, s=0, r=0, R=0, color=1;
    char *target = ".";
    for (int i = 1; args[i]; i++) {
        if (args[i][0] == '-') {
            if (strchr(args[i], 'a')) a = 1;
            if (strchr(args[i], 'l')) l = 1;
            if (strchr(args[i], 'h')) h = 1;
            if (strchr(args[i], 't')) t = 1;
            if (strchr(args[i], 'S')) s = 1;
            if (strchr(args[i], 'r')) r = 1;
            if (strchr(args[i], 'R')) R = 1;
        } else target = args[i];
    }
    list_dir(target, a, l, h, t, s, r, R, color);
    return 1;
}
