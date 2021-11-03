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
        return execute_external(shell, cmd);
    }
}

int execute_external(struct smallsh *shell, struct cmd *cmd){
    pid_t spawnpid = fork();
    switch (spawnpid) {
        case -1:
            return 1; // couldn't generate child process 
            break;
        case 0: {   // Build an argument list to pass to execvp 
                    char *argv[cmd->argc + 3];

                    // First element is the command name
                    argv[0] = malloc(sizeof(cmd->cmd));
                    strcpy(argv[0], cmd->cmd);

                    // Remaining elements (except last) are command + args
                    for (size_t i = 0; i < cmd->argc; i++) {
                        argv[i + 1] = malloc(sizeof(cmd->args[i]));
                        strcpy(argv[i + 1], cmd->args[i]);
                    }
                    argv[cmd->argc + 2] = NULL;  // Last element is null
                    execvp(argv[0], argv);
                    exit(EXIT_FAILURE); // execvp only returns if command failed
                }
        default: {
                    // Child process provides new shell status
        		    int status;
                    waitpid(spawnpid, &status, 0);
	        	    return status;
                 }
    }
} 
