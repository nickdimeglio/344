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
    int processCount;
    int status; 
    bool statusIsSignal;
    bool foregroundOnly;
};

struct processNode {
    /* doubly linked list node for smallsh processes */
    char *command;
    pid_t pid;
    int status;
    struct processNode *next;
};

int smallshExecute(struct smallsh *shell, struct cmd *cmd);
void smallshExit();
void cd(struct cmd *cmd);
void printStatus(int status);
int execute_external(struct smallsh *shell, struct cmd *cmd);
void trackProcess(struct smallsh *shell, struct cmd *cmd, pid_t pid);
void removeProcess(struct smallsh *shell, struct processNode *node);

#endif
