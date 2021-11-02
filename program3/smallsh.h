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
    int status; 
};

struct processNode {
    /* doubly linked list node for smallsh processes */
    long pid;
    int status;
    struct processNode *prev;
    struct processNode *next;
};

int cmdExecute(struct smallsh *smallsh, struct cmd *cmd);
void smallshExit();
void cd(struct cmd *cmd);
void status();
void execute_external();

#endif
