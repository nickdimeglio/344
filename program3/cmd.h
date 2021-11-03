#ifndef CMD_H
#define CMD_H
#include <stdbool.h>

/* ---------------------------------------------------
 *
 * smallsh command struct with accompanying 
 * function declarations
 *
 * -------------------------------------------------*/
struct cmd {
    /* struct for smallsh commands */
    char *text;
    char *cmd;
    char **args;
    int argc;
    bool background;
    char *input;
    char *output;
};

char* cmdExpand(char* cmdString);
struct cmd *cmdInit();
struct cmd *cmdParse(char* cmd);
void cmdPrint(struct cmd *cmd);

#endif
