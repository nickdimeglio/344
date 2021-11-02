#ifndef SMALLSH_H
#define SMALLSH_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ---------------------------------------------------
 *
 * smallsh struct with accompanying function declarations
 *
 * -------------------------------------------------*/

struct smallsh {
    /* struct for smallsh shell */
    struct cmd *lastCommand; 
    struct processNode *processesHead;
    struct processNode *processesTail;
    int processCount;
};

struct processNode {
    /* doubly linked list node for smallsh processes */
    long pid;
    int status;
    struct processDLL *prev;
    struct processDLL *next;
};

int cmdExecute(struct smallsh *smallsh, struct cmd *cmd);
void smallshExit();
void cd(struct cmd *cmd);
void status();
void execute_external();

#endif
