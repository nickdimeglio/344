#include <stdio.h>
#include "cmd.h"

/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o movies main.c
*/

int main(int argc, char *argv[]) {
    for(;;) {
        // Print prompt 
        printf("\n:");

        // Fetch command 
        char *command = calloc(2049, sizeof(char));
        scanf("%s", command);

        // Parse command 
        struct cmd *cmd = parseCommand(command);

        // Execute command
        if (cmd != NULL) {
            cmdPrint(cmd);
        }
    }
}
