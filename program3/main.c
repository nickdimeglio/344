#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "cmd.h"
#include "smallsh.h"
#include "smallsh_signals.h"

/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*/

// Global variable for handling Foreground-only mode
bool foregroundOnly = false; 

int main(int argc, char *argv[]) {

    // Initialize smallsh instance 
    struct smallsh *shell = malloc(sizeof(struct smallsh));
    shell->status = EXIT_SUCCESS;
    shell->statusIsSignal = false;
    shell->processesHead = NULL;
    shell->processCount = 0;

    // Shell process should not stop on Ctrl-C or Ctrl-Z
    ignoreSIGINT();
    ignoreSIGTSTP();

    for(;;) {
        // Check for zombie processes, remove if found
        struct processNode *node = shell->processesHead;
        while (node) {
            int status;
            bool zombie = waitpid(node->pid, &status, WNOHANG);
            if (zombie) {
                printf("background pid %d is done: ", node->pid);
                fflush(NULL);
                printStatus(shell);
                removeProcess(shell, node);
            }
            node = node->next;
        }
        // Print prompt 
        printf(": ");
        fflush(NULL);

        // Listen for Ctrl-Z and toggle foreground-only mode if needed
        handleForegroundOnly();        
        shell->foregroundOnly = foregroundOnly;

        // Fetch command
        char *command = calloc(2049, sizeof(char)); 
        fgets(command, 2049, stdin);
        command[strlen(command) - 1] = '\0'; // Trim off newline char

        // Parse command 
        struct cmd *cmd = cmdParse(shell, command);
        free(command);

        // Execute command
        if (cmd) {
            // cmdPrint(cmd);
            shell->status = smallshExecute(shell, cmd); 
        }
     }
}    


 /*----------------------------------------------------
 *                    FUNCTIONS                       |
 *                       FOR                          |
 *               FOREGROUND ONLY MODE                 |
 * --------------------------------------------------*/
void toggleFGO(int sig) {
    if (foregroundOnly) {
        // Exit Foreground-only Mode
        foregroundOnly = false;
        char* msg = "\nExiting foreground-only mode\n";
        write(STDOUT_FILENO, msg, strlen(msg));
        fflush(NULL);

    }
    else {
        // Enter Foreground-only Mode
        foregroundOnly = true;
        char* msg = "\nEntering foreground-only mode\n";
        write(STDIN_FILENO, msg, strlen(msg));
        fflush(NULL);
    }
}

void handleForegroundOnly(struct smallsh *shell) {
    /* Causes the current process (intended to be smallsh)
     * to listen for SIGSTP and enter foreground-only mode
     * when Ctrl-Z is pressed
    */
    sigset_t sigstp;
    sigaddset(&sigstp, SIGTSTP);
    struct sigaction handleForegroundOnly = {
        .sa_handler = toggleFGO,
        .sa_mask = 0,
        0,
    };
    sigaction(SIGTSTP, &handleForegroundOnly, NULL);
}

