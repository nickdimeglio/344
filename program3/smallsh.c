#include <fcntl.h>
#include <signal.h>
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
        struct processNode *node = shell->processesHead;
        while (node) {
            // close process
            node = node->next;
        }
        exit(EXIT_SUCCESS);
    } 
    // Built-in cd command
    else if (strcmp(cmd->argv[0], "cd") == 0) {
        char *path = cmd->argv[1];
        if (path) {
            chdir(path);
        } else {
            chdir(getenv("HOME"));
        }
        return shell->status;
    } 
    // Built-in status command
    else if (strcmp(cmd->argv[0], "status") == 0) {
        if (shell->statusIsSignal) {
            // Must inspect status if last process did not terminate normally
            printf("terminated by signal %d", WTERMSIG(shell->status));
        }
        else {
            printf("exit value %d\n", shell->status);
        }
        fflush(NULL);
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
            /* -----------------------------
             *          CHILD PROCESS
             *  ------------------------------ */
            
            /*  --------------------------
             * Redirect input if needed 
            ---------------------------- */ 
            char* newInput = NULL;
            if (cmd->background) {
                // Background processes default to /dev/null for input
                newInput = malloc(strlen("/dev/null"));
                strcpy(newInput, "/dev/null"); 
            }
            if (cmd->input) {
                // User specification should override default
                free(newInput);
                newInput = malloc(strlen(cmd->input));
                strcpy(newInput, cmd->input);
            }
            if (newInput) {
                FILE *input = fopen(newInput, "r");
                if (!input) {
                    printf("cannot open %s for input\n", cmd->input);
                    fflush(NULL);
                    exit(1);        // File not found
                } else {
                    int newfileno = dup2(fileno(input), STDIN_FILENO);
                    if (newfileno < 0) {
                        exit(1);    // Redirect failed
                    }
                } 
                free(newInput);
            }
            /*  -------------------------
             * Redirect output if needed
            ----------------------------- */
            char* newOutput = NULL;
            if (cmd->background) {
                // Background processes default to /dev/null for output
                newOutput = malloc(strlen("/dev/null"));
                strcpy(newOutput, "/dev/null");
            }
            if (cmd->output) {
                // User specification should override default
                free(newOutput);
                newOutput = malloc(strlen(cmd->output));
                strcpy(newOutput, cmd->output);
            }
            if (newOutput) {
                // Only update output if background or user specified
                FILE *output = fopen(newOutput, "w");
                if (!output) {
                    printf("cannot open %s for output\n", cmd->output);
                    fflush(NULL);
                    exit(1);        // File not found
                } else {
                    int newfileno = dup2(fileno(output), STDOUT_FILENO);
                    if (newfileno < 0) {
                        exit(1);    // Redirect failed
                    }
                }
                free(newOutput);
            }
            /*  -------------------------------------------
             * Must define rules for handling SIGINT, as
             * they vary between foreground and background
             * children.
            -------------------------------------------- */
            if (cmd->background) {
                // Background children ignore SIGINT
                sigset_t sigint;
                sigaddset(&sigint, SIGINT);
                struct sigaction ignoreSIGINT = { 
                    .sa_handler = SIG_IGN,
                    .sa_mask = sigint,
                    0,
                };
                sigaction(SIGINT, &ignoreSIGINT, NULL);  

            }
            else {
                // Foreground children exit on SIGINT
                sigset_t sigint;
                sigaddset(&sigint, SIGINT);
                struct sigaction exitOnSIGINT = {
                    .sa_handler = SIG_DFL,
                    .sa_mask = sigint,
                    0,
                };
                sigaction(SIGINT, &exitOnSIGINT, NULL);
            }
            execvp(cmd->argv[0], cmd->argv);
            printf("%s: no such file or directory\n", cmd->argv[0]);  // execvp only returns if command failed
            exit(1); 
        }
        default: {
            /* --------------------------
             *      PARENT PROCESS
             * ---------------------------*/
            if (!cmd->background) {
                // Pause smallsh for foreground processes 
                int status; 
                waitpid(spawnpid, &status, 0);
                if (WIFEXITED(status)) {
                    // Shell status is 0 or 1 on normal exit
                    if (WEXITSTATUS(status)) {return 1;} else {return 0;}
                }
                else {
                    // If terminated by signal, shell status is that signal
                    printf("terminated by signal %d\n", WTERMSIG(status));
                    shell->statusIsSignal = false;
                    return status;
                }
            }
            else {
                // Don't pause for background processes
                trackProcess(shell, cmd, spawnpid);
                printf("background pid is %d\n", spawnpid);
                fflush(NULL);
                return shell->status; 
            }
        } 
    }
}

void trackProcess(struct smallsh *shell, struct cmd *cmd,  pid_t pid) {
    /* 
     * Add pid to shell's linked list of background
     * processes
    */
    struct processNode* node = malloc(sizeof(struct processNode));
    node->command = malloc(strlen(cmd->text));
    strcpy(node->command, cmd->text);

    node->pid = pid;
    if (!shell->processesHead) {
        // First background process of shell 
        shell->processesHead = node;
        node->next = NULL;
    } 
    else {
        // Additional processes enter at front
        node->next = shell->processesHead;
        shell->processesHead = node;
    }

}

void removeProcess(struct smallsh *shell, struct processNode *node) {
    /*
     * Remove pid from shell's linked list of
     * background processes. Linked list should not be empty.
    */
    struct processNode *curr = shell->processesHead;
    if (curr == node) {
        // Removing head
        shell->processesHead = node->next;
        free(node);
    } else {
        while (curr->next) {
            if (curr->next == node) {
                struct processNode *temp = curr->next->next;
                free(curr->next);
                curr->next = temp;
            } else {
                curr = curr->next;
            }
        }
    }
}

