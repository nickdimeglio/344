#include <fcntl.h>
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
    if (strcmp(cmd->argv[0], "exit") == 0) {
        // Close child processes
        struct processNode *process = shell->processesHead;
        while (process) {
            // close process
            process = process->next;
        }
        exit(EXIT_SUCCESS);
    } 
    // Built-in cd command
    else if (strcmp(cmd->argv[0], "cd") == 0) {
        char *path = cmd->argv[1];
        if (cmd->argc > 0) {
            chdir(path);
        } else {
            chdir(getenv("HOME"));
        }
        return shell->status;
    } 
    // Built-in status command
    else if (strcmp(cmd->argv[0], "status") == 0) {
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
        case -1: {
            return 1; // couldn't generate child process 
            break;
        }
        case 0: {
            // Redirect stdin if needed
            if (cmd->input) {
                FILE *input = fopen(cmd->input, "r");
                if (!input) {
                    printf("cannot open %s for input\n", cmd->input);
                    fflush(NULL);
                    exit(1);
                } else {
                    int newfileno = dup2(fileno(input), STDIN_FILENO);
                    if (newfileno < 0) {
                    	exit(1); // Redirect failed, return smallsh failure status 
                    }
                } 
            }
            // Redirect stdout if needed
            if (cmd->output) {
                FILE *output = fopen(cmd->output, "w");
                if (!output) {
                    printf("cannot open %s for output\n", cmd->output);
		    fflush(NULL);
                } else {
                    int newfileno = dup2(fileno(output), STDOUT_FILENO);
                    if (newfileno < 0) {
                        exit(1); // Redirect failed, return smallsh failure status 
                    }
                }
            execvp(cmd->argv[0], cmd->argv);
            exit(1); // execvp only returns if command failed
            }
        }
        default: {
            // Child process provides new shell status
            int status;
            waitpid(spawnpid, &status, 0);
            if (status) {
                // Non-zero status means execvp failed
                return 1;
            } else {
                return 0;
            }
            return status;
        }
    }
} 
