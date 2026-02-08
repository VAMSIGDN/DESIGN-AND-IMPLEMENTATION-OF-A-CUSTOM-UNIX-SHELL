#include "shell.h"

// Helper function to handle the "head" logic
int run_head(char **args) {
    int lines = 10; // Default
    char *filename;

    if (args[1] != NULL && strcmp(args[1], "-n") == 0) {
        if (args[2] != NULL) {
            lines = atoi(args[2]);
            filename = args[3];
        }
    } else {
        filename = args[1];
    }

    if (filename == NULL) {
        printf("Usage: head [-n lines] <filename>\n");
        return 1;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("head"); return 1; }

    char buffer[1024];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), fp) && count < lines) {
        printf("%s", buffer);
        count++;
    }
    fclose(fp);
    return 1;
}

// Helper function to handle the "tail" logic
int run_tail(char **args) {
    int lines = 10; // Default
    char *filename;

    if (args[1] != NULL && strcmp(args[1], "-n") == 0) {
        if (args[2] != NULL) {
            lines = atoi(args[2]);
            filename = args[3];
        }
    } else {
        filename = args[1];
    }

    if (filename == NULL) {
        printf("Usage: tail [-n lines] <filename>\n");
        return 1;
    }

    // Advanced logic: Read file twice or use a circular buffer
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("tail"); return 1; }

    char buffer[1024];
    int total_lines = 0;
    while (fgets(buffer, sizeof(buffer), fp)) total_lines++;
    
    fseek(fp, 0, SEEK_SET);
    int start_at = total_lines - lines;
    int current = 0;
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (current >= start_at) printf("%s", buffer);
        current++;
    }
    fclose(fp);
    return 1;
}

// Unified entry point for the executor
int cmd_head_tail(char **args) {
    if (strcmp(args[0], "head") == 0) return run_head(args);
    if (strcmp(args[0], "tail") == 0) return run_tail(args);
    return 1;
}
