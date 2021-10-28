#include <stdio.h>
#include "cmd.c"

/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o movies main.c
*/

int main(int argc, char *argv[]) {
    for(;;) {
        // Print prompt 
        printf(":");

        // Fetch command
        char *command = calloc(2049, sizeof(char)); 
        fgets(command, 2049, stdin);

        // Parse command 
        struct cmd *cmd = parseCommand(command);

        // Execute command
        if (cmd) {
            cmdPrint(cmd);

        // Free command memory
        free(command);
        free(cmd);
        }

    }    
}
