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
    /* struct for smallsh */
    struct cmd *lastCommand; 
    struct processNode *processesHead;
    struct processNode *processesTail;
    int processCount;
    int status; 
};

struct processNode {
    /* doubly linked list node for smallsh processes */
    char *command;
    pid_t pid;
    int status;
    struct processNode *prev;
    struct processNode *next;
};

int smallshExecute(struct smallsh *smallsh, struct cmd *cmd);
void smallshExit();
void cd(struct cmd *cmd);
void status();
int execute_external(struct smallsh *shell, struct cmd *cmd);
void trackProcess(struct smallsh *shell, struct cmd *cmd, pid_t pid);
void removeProcess(struct processNode *node);

#endif
