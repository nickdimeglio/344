#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"
#include "smallsh.h"

/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*/

int main(int argc, char *argv[]) {

    // Initialize smallsh instance
    struct smallsh *shell = malloc(sizeof(struct smallsh));
    shell->lastCommand = NULL;
    shell->status = EXIT_SUCCESS;
    shell->processesHead = NULL;
    shell->processesTail = NULL;
    shell->processCount = 0;

    for(;;) {
        // Check for zombie processes
        struct processNode *node = shell->processesHead;
        while (node) {
            printf("Checking this command with pid %d: %s", node->pid, node->command);
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

        // Execute command
        if (cmd) {
            // cmdPrint(cmd);
            shell->status = smallshExecute(shell, cmd); 
        }

        // Free command string and previous shell command
        free(command);
        free(shell->lastCommand);
        shell->lastCommand = cmd;
     }
}    
