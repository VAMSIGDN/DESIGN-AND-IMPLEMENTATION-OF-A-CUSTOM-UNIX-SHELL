#include "shell.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h> 

#define MAX_TRACK 50
#define SUGGEST_THRESHOLD 3

typedef struct {
    char cmd_str[256];
    int count;
} CommandTracker;

static CommandTracker history[MAX_TRACK];
static int track_count = 0;

int count_words(const char *str) {
    int count = 0;
    int in_word = 0;
    while (*str) {
        if (isspace((unsigned char)*str)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
        str++;
    }
    return count;
}

void analyze_and_suggest(char *full_cmd) {
    if (full_cmd == NULL || strlen(full_cmd) < 3) return;

    char clean_cmd[256];
    strncpy(clean_cmd, full_cmd, 255);
    clean_cmd[255] = '\0';

    // NEW ROBUST TRIM: Strip Newlines, Tabs, and Spaces from the end
    size_t len = strlen(clean_cmd);
    while (len > 0 && (clean_cmd[len - 1] == '\n' || 
                       clean_cmd[len - 1] == '\r' || 
                       isspace((unsigned char)clean_cmd[len - 1]))) {
        clean_cmd[--len] = '\0';
    }

    int words = count_words(clean_cmd);
    
    // DEBUG: Uncomment the line below to see what the shell is actually "seeing"
    // printf("[Debug] Words: %d | Cmd: '%s'\n", words, clean_cmd);

    if (words <= 4) return; 

    for (int i = 0; i < track_count; i++) {
        if (strcmp(history[i].cmd_str, clean_cmd) == 0) {
            history[i].count++;
            if (history[i].count >= SUGGEST_THRESHOLD) {
                printf("\033[1;35m\n[Smart Tip]\033[0m You've used this complex command %d times.\n", 
                        history[i].count);
                printf("Consider creating an alias: alias name=\"%s\"\n\n", clean_cmd);
                history[i].count = 0; 
            }
            return;
        }
    }

    if (track_count < MAX_TRACK) {
        strncpy(history[track_count].cmd_str, clean_cmd, 255);
        history[track_count].count = 1;
        track_count++;
    }
}
