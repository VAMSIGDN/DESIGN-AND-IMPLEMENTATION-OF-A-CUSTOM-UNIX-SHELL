#include "shell.h"
#include <ctype.h>

/**
 * cmd_cat - Custom implementation of the 'cat' command.
 * Supports:
 * - cat <file1> <file2> ... (Concatenation)
 * - cat -n <file> (Line numbering)
 * - cat -s <file> (Squeeze blank lines)
 */
int cmd_cat(char **args) {
    int line_numbering = 0;
    int squeeze_blank = 0;
    int file_index = 1;

    // 1. Parse Flags
    while (args[file_index] != NULL && args[file_index][0] == '-') {
        if (strcmp(args[file_index], "-n") == 0) line_numbering = 1;
        else if (strcmp(args[file_index], "-s") == 0) squeeze_blank = 1;
        file_index++;
    }

    // If no files provided, cat usually reads from stdin (for this shell, we show usage)
    if (args[file_index] == NULL) {
        printf("Usage: cat [-n] [-s] <file1> [file2...]\n");
        return 1;
    }

    // 2. Loop through all provided files
    for (int i = file_index; args[i] != NULL; i++) {
        FILE *fp = fopen(args[i], "r");
        if (!fp) {
            perror("myshell cat");
            continue;
        }

        char buffer[1024];
        int line_count = 1;
        int last_was_blank = 0;

        while (fgets(buffer, sizeof(buffer), fp)) {
            int is_blank = (buffer[0] == '\n' || buffer[0] == '\r');

            // Handle -s (Squeeze multiple blank lines into one)
            if (squeeze_blank && is_blank && last_was_blank) {
                continue;
            }

            // Handle -n (Print line numbers)
            if (line_numbering) {
                printf("%6d  ", line_count++);
            }

            printf("%s", buffer);
            last_was_blank = is_blank;
        }
        fclose(fp);
    }

    return 1;
}
