#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "cmd.h"

/* ---------------------------------------------------
 *
 *  functions for the smallsh cmd struct
 *
 * --------------------------------------------------*/
char* cmdExpand(char* cmdString) {
    /* for each arg in the command, expand all
     * instances of "$$" into the process ID of smallsh
    */

    // Get smallsh's PID as string for variable expansion
    long pid = getpid();
    char *smallshPID = calloc(32, sizeof(char));
    sprintf(smallshPID, "%ld", pid);

    // Expanded string may be longer, allocate new memory
    char *expanded = calloc(2049, sizeof(char));

    // Copy over each char from cmdString, or PID if "$$" is encountered
    int i = 0;
    int j = 0;
    while (i < strlen(cmdString)) {
        if (cmdString[i] == '$' && cmdString[i] == cmdString[i + 1]) {
            // Expand "$$" 
            strcat(expanded, smallshPID);
            // Must realign string indices to continue
            j += strlen(smallshPID);
            i += 2;
        } else {
            // No expansion needed
            expanded[j] = cmdString[i];
            i++;
            j++;
        }
    }
    free(smallshPID);
    return expanded;
}

struct cmd *cmdInit() {
    /* initialize a new instance of the command struct
    */
    struct cmd *cmd = malloc(sizeof(struct cmd)); 
     
    // Input defaults to stdin
    cmd->input = malloc(strlen("stdin") + 1);
    strcpy(cmd->input, "stdin");

    // Output defaults to stdout
    cmd->output = malloc(strlen("stdout") + 1);
    strcpy(cmd->output, "stdout");

    // cmd executes in foreground by default
    cmd->background = false; 

    return cmd;
}

struct cmd *cmdParse(char* cmdString) {
    /* parse a string into a cmd struct
    */
   
    // Identify blank commands since they can be skipped
    bool isBlank = true;
    for (size_t i = 0; i < strlen(cmdString); i++) {
        if (!(isspace(cmdString[i]) || isblank(cmdString[i]))) {
            isBlank = false;
        }
    }

    // Don't parse if command is blank or a comment (leads with #) 
    if (isBlank || cmdString[0] == '#') { return NULL; }

    // Initialize struct to separate command from args and options
    struct cmd *cmd = cmdInit();
    cmd->argc = 0;

    // Expand the variable $$
    cmdString = cmdExpand(cmdString);

    // Store the full command text 
    cmd->text = calloc(strlen(cmdString) + 1, sizeof(char));
    strcpy(cmd->text, cmdString);


    /* ----------------------------------
     * Tokenize the text to retrieve the
     * the command and any args or options
     * ----------------------------------- */

    char* saveptr; 
    char* token = strtok_r(cmdString, " ", &saveptr);

    // Store the command
    cmd->cmd = calloc(strlen(token) + 1, sizeof(char));
    strcpy(cmd->cmd, token);

    // Store arguments and options
    token = strtok_r(NULL, " ", &saveptr);
    while(token) {
        // Option: Input redirection
        if (strcmp(token, "<") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            free(cmd->input);
            cmd->input = malloc(strlen(token) + 1);
            strcpy(cmd->input, token);
        }
        // Option: Output redirection
        else if (strcmp(token, ">") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            free(cmd->output);
            cmd->output = malloc(strlen(token) + 1);
            strcpy(cmd->output, token);
        }
        // Option: Background process
        // (must be at end of command string)
        else if (strcmp(token, "&") == 0 && (!saveptr || *saveptr == '\0')) {
            cmd->background = true;
        }
        // New Argument
        else {
            cmd->args[cmd->argc] = malloc(strlen(token) + 1);
            strcpy(cmd->args[cmd->argc], token);
            cmd->argc++;
        }
        // Get next argument or option
        token = strtok_r(NULL, " ", &saveptr);
    } 
    return cmd;
}

void cmdPrint(struct cmd *cmd) {
    /* print the command in a digestable format
    */
    printf("\nText Entered: %s", cmd->text);
    printf("\nCommand: %s", cmd->cmd);
    if (cmd->argc > 1) {
        printf("\nArgs:");
        for (size_t i = 0; i < cmd->argc; i++) {
            printf("    %zu: %s", i, cmd->args[i]);
        }
    } else {printf("\nArgs: None");}
    printf("\nInput: %s", cmd->input);
    printf("\nOutput: %s", cmd->output);
    printf("\nBackground Process: %s\n", cmd->background ? "Yes" : "No");
}
