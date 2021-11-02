#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"
#include "smallsh.h"

/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o movies main.c
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
        // Print prompt 
        printf(": ");

        // Fetch command
        char *command = calloc(2049, sizeof(char)); 
        fgets(command, 2049, stdin);
        command[strlen(command) - 1] = '\0'; // Trim off newline char

        // Parse command 
        struct cmd *cmd = cmdParse(command);

        // Execute command
        if (cmd) {
            shell->status = smallshExecute(shell, cmd); 
        }

        // Free command memory
        free(command);
        free(cmd);
     }
}    
