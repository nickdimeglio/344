#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cmd.h"
#include "smallsh.h"

/* ---------------------------------------------------
 *
 * smallsh struct with accompanying functions 
 *
 * -------------------------------------------------*/
struct processDLL {
    /* doubly linked list of smallsh processes */
    long pid;
    int status;
    struct processDLL *prev;
    struct processDLL *next;
};
 
int cmdExecute(struct smallsh *smallsh, struct cmd *cmd) {
    /* TODO: Add status() and execute_external(). Add
     *       background processing.
     *
     * Execute the provided smallsh cmd using built-in
     * commands or commands found with the $PATH variable
     * Returns exit status from the executed command
    */
    if (strcmp(cmd->cmd, "exit") == 0) {
        // Call built-in exit 
        // smallshExit();
        // return status();
        return 0;
    } else if (strcmp(cmd->cmd, "cd") == 0) {
        // Call built-in cd
        // cd(cmd);    
        // return status();
        return 0;
    } else if (strcmp(cmd->cmd, "status") == 0) {
        // Call built-in status
        // return status();
        return 0;
    } else {
        // Call non-built-in command 
        // return execute_external(cmd);
        return 0;
    }
}

void smallshExit() {
    /* TODO: Implement smallshExit()
     *
     * Kill any existing processes or jobs the shell
     * has started and terminate the shell.
    */
       
    // Loop through the shell's processes and kill
    
    // Terminate the shell
}

void cd(struct cmd *cmd) {
    // cd([path]) -> void
    //      if path:
    //          change to directory specified by path argument
    //      else:
    //          change to directory specified by $HOME
}

void status() {
// status() -> void
//      ignore built-in commands
//
//      print out either the exit status or the terminating 
//      signal of the last foreground process ran by the shell
//
//      if the command is run before any foreground command is run,
//      return exit status 0
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

