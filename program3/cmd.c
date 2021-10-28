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
    printf("%s", cmd->text);
}

/* parse a provided string into a cmd */
struct cmd *parseCommand(char* cmdString) {
    
    // Don't parse if the command is a comment 
    if (*cmdString == '#') {
        return NULL;
    }

    // Don't parse if the command is a blank line
    bool isBlank = true;

    // Initalize a cmd struct
    struct cmd *cmd = malloc(sizeof(struct cmd)); 

    // Store the command text 
    cmd->text = calloc(strlen(cmdString) + 1, sizeof(char));
    strcpy(cmd->text, cmdString);

    return cmd;
}

