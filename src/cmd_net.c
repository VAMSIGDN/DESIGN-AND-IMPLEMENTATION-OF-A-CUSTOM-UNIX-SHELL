#include "shell.h"
#include <sys/wait.h>

/**
 * cmd_net - A unified handler for all Linux networking commands.
 * Supports: ip, ping, traceroute, mtr, nmcli, ss, dig, host, tcpdump, nmap, etc.
 */
int cmd_net(char **args) {
    if (args[0] == NULL) {
        return 1;
    }

    /* * This logic handles the following categorized tools:
     * 1. Modern Essentials: ip, ss
     * 2. Connectivity: ping, traceroute, mtr, nmcli
     * 3. DNS/Domain: dig, host, nslookup, whois
     * 4. Analysis: tcpdump, nmap
     */
    
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: Execute the command with all flags/arguments
        // This allows 'ip addr', 'ping -c 4', and 'nmcli device wifi list' to work.
        if (execvp(args[0], args) == -1) {
            fprintf(stderr, "myshell: networking command '%s' not found or failed\n", args[0]);
            exit(EXIT_FAILURE);
        }
    } else if (pid > 0) {
        // Parent process: Wait for the networking tool to finish
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork");
    }

    return 1;
}
