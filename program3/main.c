#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "cmd.h"
#include "smallsh.h"

/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*/

int main(int argc, char *argv[]) {

    // Ensure the smallsh process cannot be terminated with CTRL-C 
    sigset_t sigint;
    sigaddset(&sigint, SIGINT);
    struct sigaction ignoreSIGINT = { 
        .sa_handler = SIG_IGN,
        .sa_mask = sigint,
        0,
    };
    sigaction(SIGINT, &ignoreSIGINT, NULL);

    // Initialize smallsh instance
    struct smallsh *shell = malloc(sizeof(struct smallsh));
    shell->status = EXIT_SUCCESS;
    shell->processesHead = NULL;
    shell->processCount = 0;

    for(;;) {
        // Check for zombie processes, remove if found
        struct processNode *node = shell->processesHead;
        while (node) {
            int status;
            bool zombie = waitpid(node->pid, &status, WNOHANG);
            if (zombie) {
                printf("background pid %d is done: ", node->pid);
                fflush(NULL);
                if (WIFEXITED(status)) {
                    // Child terminated normally
                    printf("exit value %d\n", WEXITSTATUS(status));
                }
                else {
                    // Child terminated because of a signal
                    printf("terminated by signal %d\n", WTERMSIG(status));
                }
                fflush(NULL);
                removeProcess(shell, node);
            }
            node = node->next;
        }
        // Print prompt 
        printf(": ");
        fflush(NULL);

        // Fetch command
        char *command = calloc(2049, sizeof(char)); 
        fgets(command, 2049, stdin);
        command[strlen(command) - 1] = '\0'; // Trim off newline char

        // Parse command 
        struct cmd *cmd = cmdParse(command);
        free(command);

        // Execute command
        if (cmd) {
            // cmdPrint(cmd);
            shell->status = smallshExecute(shell, cmd); 
        }
     }
}    
