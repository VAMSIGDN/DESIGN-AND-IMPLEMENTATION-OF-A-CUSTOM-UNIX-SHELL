#include "shell.h"
#include <termios.h>

// Helper to read a single character without needing 'Enter'
char get_input() {
    char ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int cmd_pager(char **args) {
    if (args[1] == NULL) {
        printf("Usage: %s <filename>\n", args[0]);
        return 1;
    }

    FILE *fp = fopen(args[1], "r");
    if (!fp) {
        perror("pager error");
        return 1;
    }

    char buffer[1024];
    int line_count = 0;
    int screen_height = 20; // Default height before pause

    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
        line_count++;

        if (line_count >= screen_height) {
            printf("\033[7m-- More (Press Space to continue, q to quit) --\033[0m");
            char c = get_input();
            
            // Clear the "-- More --" line
            printf("\r                                                \r");
            
            if (c == 'q' || c == 'Q') {
                break;
            }
            line_count = 0; // Reset for next "page"
        }
    }

    fclose(fp);
    return 1;
}
