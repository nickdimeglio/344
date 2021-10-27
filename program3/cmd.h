#include <stdlib.h>
#include <string.h>

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
    printf("n%s", cmd->text);
}

/* parse a provided string into a cmd */
struct cmd *parseCommand(char* cmdString) {
    struct cmd *cmd = malloc(sizeof(struct cmd)); 
    cmd->text = calloc(strlen(cmdString) + 1, sizeof(char));
    strcpy(cmd->text, cmdString);

    return cmd;
}

