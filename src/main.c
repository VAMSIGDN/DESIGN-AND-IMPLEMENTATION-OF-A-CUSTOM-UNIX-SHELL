#include "shell.h"
#include <signal.h>
volatile sig_atomic_t keep_running=1;

void handle_sigint(int sig) {
    (void)sig;
    keep_running=0;
    printf("\n%s[Interrupt] Use 'exit' to quit shell%s\n", COLOR_PROMPT, COLOR_RESET);
//    printf("myshell> ");
    fflush(stdout);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    char *line;
    char **args;
    int status;

    // Register Signal Handler for Ctrl+C
    signal(SIGINT, handle_sigint);

    do {
        keep_running=1;
        printf("%smyshell> %s", COLOR_PROMPT, COLOR_RESET);
        line = read_line();
	analyze_and_suggest(line);
        args = parse_input_with_alias(line);
        status = execute_command(args);
	
        free(line);
        free(args);
    } while (status);

    return EXIT_SUCCESS;
}

char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0; 
    
    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            printf("\n%sExiting myshell... [Ctrl+D]%s\n", COLOR_INFO, COLOR_RESET);
            exit(EXIT_SUCCESS); 
        } else {
            perror("read_line");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}
