#include "shell.h"
#include <sys/stat.h>

/**
 * cmd_file - Custom implementation to determine file types.
 * Logic:
 * 1. Use stat() to check file mode (Directory vs Regular).
 * 2. If regular, check extension and contents for specific types.
 */
int cmd_file(char **args) {
    if (args[1] == NULL) {
        printf("Usage: file <filename>\n");
        return 1;
    }

    struct stat st;
    if (stat(args[1], &st) == -1) {
        perror("myshell file");
        return 1;
    }

    printf("%s: ", args[1]);

    // 1. Check if it's a Directory
    if (S_ISDIR(st.st_mode)) {
        printf("directory\n");
    } 
    // 2. Check if it's a Regular File
    else if (S_ISREG(st.st_mode)) {
        
        // 2a. Check for Executable permissions
        if (st.st_mode & S_IXUSR) {
            printf("executable binary\n");
        }
        // 2b. Check for C Source files by extension
        else if (strstr(args[1], ".c") != NULL) {
            printf("C source text\n");
        }
        else if (strstr(args[1], ".h") != NULL) {
            printf("C header text\n");
        }
        // 2c. Check for generic text files
        else {
            // Check first few bytes for non-printable characters (simple binary check)
            FILE *fp = fopen(args[1], "rb");
            if (fp) {
                int c = fgetc(fp);
                if (c == 0x7F) printf("ELF 64-bit LSB executable\n"); // Magic number for Linux binaries
                else printf("ASCII text\n");
                fclose(fp);
            } else {
                printf("regular file\n");
            }
        }
    } else {
        printf("special file type\n");
    }

    return 1;
}
