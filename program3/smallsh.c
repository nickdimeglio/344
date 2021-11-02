#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "cmd.h"
#include "smallsh.h"

/* ---------------------------------------------------
 *
 * functions for the smallsh struct 
 *
 * -------------------------------------------------*/

int smallshExecute(struct smallsh *shell, struct cmd *cmd) {
    /* TODO: Add status() and execute_external(). Add
     *       background processing.
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
        char *dir = calloc(256, sizeof(char));
        char *newdir = calloc(256, sizeof(char));
        getcwd(dir, 256);

        if (cmd->argc > 0) {
            chdir(path);
        }
        else {
            chdir(getenv("HOME"));
        }
        
        getcwd(newdir, 256);
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

void execute_external(){
// excecute_external() -> void
//      fork off a child
//          use the PATH variable to look for non-built in comamnds (allow shell scripts)
//          use a function from the exec() family to run the command
//          if command fails
//              print error message
//              set exit status to 1
//      terminate the child process
//
//      set exit status
} 

