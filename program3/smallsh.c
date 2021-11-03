#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "cmd.h"
#include "smallsh.h"

/* ---------------------------------------------------
 *
 * functions for the smallsh struct 
 *
 * -------------------------------------------------*/

int smallshExecute(struct smallsh *shell, struct cmd *cmd) {
    /* TODO: Close child processes
     *
     * Execute the provided smallsh cmd using built-in
     * commands or commands found with the $PATH variable
     * Returns exit status from the executed command
    */

    // Built-in exit command
    if (strcmp(cmd->cmd, "exit") == 0) {
        // Close child processes
        struct processNode *process = shell->processesHead;
        while (process) {
            // close process
            process = process->next;
        }
        exit(EXIT_SUCCESS);
    } 
    // Built-in cd command
    else if (strcmp(cmd->cmd, "cd") == 0) {
        char *path = cmd->args[0];
        if (cmd->argc > 0) {
            chdir(path);
        } else {
            chdir(getenv("HOME"));
        }
        return shell->status;
    } 
    // Built-in status command
    else if (strcmp(cmd->cmd, "status") == 0) {
        printf("exit value %d\n", shell->status);
        return shell->status;
    } 
    // Non-built-in command
    else {
        // Call non-built-in command 
        // return execute_external(cmd);
        return 0;
    }
}

void execute_external(struct smallsh *shell, struct cmd *cmd){
    pid_t spawnpid = fork();
    switch (spawnpid) {
        case -1:
            shell->status = 1;
            break;
        case 0: {   // Build an argument list to pass to execlp 
                    char **argv = calloc(cmd->argc + 2, 2049);
                    strcpy(argv[0], cmd->cmd);
                    for (size_t i = 0; i < cmd->argc; i++) {
                        argv[i + 1] = cmd->args[i];
                    }
                    execvp(argv[0], argv);
                    shell->status = 1; // execlp only returns on failure
                    break;
                }
        default: {
                    // Child process provides new shell status
                    waitpid(spawnpid, &(shell->status), 0);
                    shell->status = 0; 
                    break;
                 }
    }
//          if command fails
//              print error message
//              set exit status to 1
//      terminate the child process
//
//      set exit status
} 
