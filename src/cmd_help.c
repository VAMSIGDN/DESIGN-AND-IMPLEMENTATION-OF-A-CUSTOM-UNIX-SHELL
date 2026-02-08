#include "shell.h"

/**
 * cmd_help - Custom help system for the modular shell.
 * It provides a categorized list of all implemented "perfect" commands.
 */
int cmd_help(char **args) {
    (void)args;
    printf("\n\033[1;36m====================================================\033[0m\n");
    printf("\033[1;36m             MY CUSTOM MODULAR SHELL HELP           \033[0m\n");
    printf("\033[1;36m====================================================\033[0m\n");

    printf("\n\033[1;33m[Navigation & Environment]\033[0m\n");
    printf("  cd <path>    : Change directory. Supports '~' (home) and '-' (prev).\n");
    printf("  pwd [-L/-P]  : Print working directory (Logical or Physical).\n");

    printf("\n\033[1;33m[File Listing - Custom ls]\033[0m\n");
    printf("  ls [-a] [-l] : List files with colors:\n");
    printf("                 \033[1;32mGreen (.c)\033[0m, \033[1;34mBlue (Dir)\033[0m, \033[1;35mMagenta (Exec)\033[0m\n");

    printf("\n\033[1;33m[Text Processing & Viewing]\033[0m\n");
    printf("  cat [-n] [-s]: Concatenate files. -n (numbering), -s (squeeze blanks).\n");
    printf("  head [-n N]  : View first N lines of a file.\n");
    printf("  tail [-n N]  : View last N lines of a file.\n");
    printf("  more / less  : View file one screen at a time (Space=Next, Q=Quit).\n");

    printf("\n\033[1;33m[System Information]\033[0m\n");
    printf("  stat <file>  : Display detailed file status and metadata.\n");
    printf("  file <file>  : Determine file type.\n");
    printf("  which / whereis: Locate a command binary path.\n");

    printf("\n\033[1;33m[Shell Control]\033[0m\n");
    printf("  --help       : Display this manual.\n");
    printf("  exit         : Terminate the shell session.\n");
    
    printf("\n\033[1;36m====================================================\033[0m\n\n");
    return 1;
}
