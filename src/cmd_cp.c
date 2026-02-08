#include "shell.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

/**
 * copy_file_logic - Core function to copy data and handle -u and -i flags
 */
int copy_file_logic(const char *src, const char *dest, int i_flag, int v_flag, int u_flag) {
    struct stat st_src, st_dest;
    
    if (stat(src, &st_src) != 0) { perror("cp: src"); return -1; }

    // Handle -u (Update): Only copy if src is newer than dest
    if (u_flag && stat(dest, &st_dest) == 0) {
        if (st_src.st_mtime <= st_dest.st_mtime) return 0; 
    }

    // Handle -i (Interactive): Ask before overwriting
    if (i_flag && access(dest, F_OK) == 0) {
        printf("cp: overwrite '%s'? (y/n): ", dest);
        char c = getchar(); while(getchar() != '\n'); 
        if (c != 'y' && c != 'Y') return 0;
    }

    int src_fd = open(src, O_RDONLY);
    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, st_src.st_mode);
    
    char buffer[4096];
    ssize_t bytes;
    while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
        write(dest_fd, buffer, bytes);
    }

    if (v_flag) printf("'%s' -> '%s'\n", src, dest);

    close(src_fd);
    close(dest_fd);
    return 0;
}

/**
 * copy_recursive - Implementation for the -r flag
 */
void copy_recursive(const char *src, const char *dest, int i_flag, int v_flag, int u_flag) {
    struct stat st;
    stat(src, &st);

    if (S_ISDIR(st.st_mode)) {
        mkdir(dest, st.st_mode);
        DIR *d = opendir(src);
        struct dirent *p;
        while ((p = readdir(d))) {
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) continue;
            char s_path[1024], d_path[1024];
            snprintf(s_path, 1024, "%s/%s", src, p->d_name);
            snprintf(d_path, 1024, "%s/%s", dest, p->d_name);
            copy_recursive(s_path, d_path, i_flag, v_flag, u_flag);
        }
        closedir(d);
    } else {
        copy_file_logic(src, dest, i_flag, v_flag, u_flag);
    }
}

int cmd_cp(char **args) {
    int r_flag = 0, i_flag = 0, v_flag = 0, u_flag = 0;
    int idx = 1;

    while (args[idx] && args[idx][0] == '-') {
        if (strchr(args[idx], 'r')) r_flag = 1;
        if (strchr(args[idx], 'i')) i_flag = 1;
        if (strchr(args[idx], 'v')) v_flag = 1;
        if (strchr(args[idx], 'u')) u_flag = 1;
        idx++;
    }

    if (!args[idx] || !args[idx+1]) {
        printf("cp: missing file operand\n");
        return 1;
    }

    if (r_flag) {
        copy_recursive(args[idx], args[idx+1], i_flag, v_flag, u_flag);
    } else {
        struct stat st;
        if (stat(args[idx], &st) == 0 && S_ISDIR(st.st_mode)) {
            printf("cp: -r not specified; omitting directory '%s'\n", args[idx]);
        } else {
            copy_file_logic(args[idx], args[idx+1], i_flag, v_flag, u_flag);
        }
    }
    return 1;
}
