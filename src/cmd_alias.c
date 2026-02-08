#include "shell.h"

#define MAX_ALIASES 50

typedef struct {
    char name[32];
    char value[MAX_LINE];
} Alias;

Alias alias_table[MAX_ALIASES];
int alias_count = 0;

/**
 * Global Lookup: Used by parser.c to expand commands.
 */
char *get_alias_value(char *key) {
    if (key == NULL) return NULL;
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias_table[i].name, key) == 0) {
            return alias_table[i].value;
        }
    }
    return NULL; 
}

/**
 * Built-in: alias
 */
int cmd_alias(char **args) {
    // 1. THE LISTING LOGIC
    if (args[1] == NULL) {
        if (alias_count == 0) {
            printf("No aliases defined.\n");
            return 1;
        }
        printf("\n\033[1;36m--- Active Aliases ---\033[0m\n");
        printf("%-15s | %-s\n", "Shortcut", "Command");
        printf("----------------|------------------\n");
        for (int i = 0; i < alias_count; i++) {
            printf("%-15s | %-s\n", alias_table[i].name, alias_table[i].value);
        }
        return 1;
    }

    // 2. THE DEFINING LOGIC (name=value)
    // FIX: Recombine arguments to capture spaces (e.g., alias ll=ls -l)
    char full_input[MAX_LINE] = "";
    for (int i = 1; args[i] != NULL; i++) {
        strcat(full_input, args[i]);
        if (args[i+1] != NULL) strcat(full_input, " ");
    }

    // FIX: Use 'full_input' instead of the undeclared 'input'
    char *equals_sign = strchr(full_input, '=');
    if (equals_sign == NULL) {
        fprintf(stderr, "Error: Use format name=value (e.g., alias la=ls -a)\n");
        return 1;
    }

    *equals_sign = '\0';
    char *name = full_input;
    char *value = equals_sign + 1;
    
    // Clean quotes if user typed: alias ll="ls -l"
    if (value[0] == '\"') {
        value++;
        if (value[strlen(value) - 1] == '\"') {
           value[strlen(value) - 1] = '\0';
        }
    }

    // Update if exists, otherwise add new
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias_table[i].name, name) == 0) {
            strncpy(alias_table[i].value, value, MAX_LINE - 1);
            printf("Updated alias '%s'\n", name);
            return 1;
        }
    }

    if (alias_count < MAX_ALIASES) {
        strncpy(alias_table[alias_count].name, name, 31);
        strncpy(alias_table[alias_count].value, value, MAX_LINE - 1);
        alias_count++;
        printf("Set alias '%s'\n", name);
    }
    return 1;
}

/**
 * Built-in: unalias
 */
int cmd_unalias(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "unalias: missing argument\n");
        return 1;
    }
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias_table[i].name, args[1]) == 0) {
            for (int j = i; j < alias_count - 1; j++) {
                alias_table[j] = alias_table[j + 1];
            }
            alias_count--;
            printf("Removed alias '%s'.\n", args[1]);
            return 1;
        }
    }
    printf("unalias: %s not found.\n", args[1]);
    return 1;
}
