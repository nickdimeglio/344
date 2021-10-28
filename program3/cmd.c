#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* smallsh command struct with accompanying functions */
struct cmd;
struct cmd *parseCommand(char* cmd);

/* struct for smallsh commands */
struct cmd {
    char *text;
};

/* print the command's original text entered
 * by the user
*/
void cmdPrint(struct cmd *cmd) {
    printf("%s\n", cmd->text);
}

/* parse a provided string into a cmd */
struct cmd *parseCommand(char* cmdString) {
    
    // Check if command is a blank
    bool isBlank = true;
    for (size_t i = 0; i < strlen(cmdString); i++) {
        if (!isspace(cmdString[i])) {
            isBlank = false;
        }
    }

    // Don't parse if line is blank or leads with # 
    if (isBlank || cmdString[0] == '#') {
        return NULL;
    }

    // Initalize a cmd struct
    struct cmd *cmd = malloc(sizeof(struct cmd)); 

    // Store the command text 
    cmd->text = calloc(strlen(cmdString) + 1, sizeof(char));
    strcpy(cmd->text, cmdString);

    return cmd;
}

