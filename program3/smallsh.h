#ifndef SMALLSH_H
#define SMALLSH_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ---------------------------------------------------
 *
 * smallsh struct with accompanying functions 
 *
 * -------------------------------------------------*/

struct smallsh {
    /* struct for smallsh shell */
    struct cmd *lastCommand; 
    struct processDLL *processes;
    int processCount;
};

struct processDLL;
int cmdExecute(struct smallsh *smallsh, struct cmd *cmd);
void smallshExit();
void cd(struct cmd *cmd);
void status();
void execute_external();

#endif
