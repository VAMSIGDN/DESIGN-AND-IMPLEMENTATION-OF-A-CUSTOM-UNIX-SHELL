#include "shell.h"

// THE WRAPPER: Pre-processes the input for Feature 4 and Alias expansion.
char **parse_input_with_alias(char *line) {
    if (line == NULL || strlen(line) == 0) return NULL;

    size_t len = strlen(line);
    while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r' || line[len-1] == ' ')) {
        line[--len] = '\0';
    }
    if (len == 0) return NULL;

    // Call logic defined in cmd_suggest.c
    analyze_and_suggest(line);

    char *first_word = strdup(line);
    char *space_in_original = strchr(line, ' ');
    char *space_ptr = strchr(first_word, ' ');
    if (space_ptr) *space_ptr = '\0';

    char *expanded = get_alias_value(first_word); 

    if (expanded != NULL) {
        char *new_line = calloc(MAX_LINE, sizeof(char));
        strcpy(new_line, expanded); // Start with the expanded command (e.g., "ls -l")

       
        // space_in_original includes the leading space (e.g., " /home")
        if (space_in_original) {
            strcat(new_line, space_in_original);
        }

        free(first_word);
        char **tokens = parse_line(new_line);
        free(new_line);
        return tokens;
    }

    free(first_word);
    return parse_line(line);
}

// TOKENIZATION
char **parse_line(char *line) {
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "myshell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    char *line_copy = strdup(line);
    token = strtok(line_copy, TOK_DELIM);
    
    while (token != NULL) {
       
        tokens[position] = strdup(token); 
        position++;

        if (position >= bufsize) {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "myshell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOK_DELIM);
    }
    tokens[position] = NULL;
    free(line_copy); 
    return tokens;
}
