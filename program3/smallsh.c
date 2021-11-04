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
            /* Redirect input if needed 
            ----------------------------*/ 
            char* newInput = NULL;
            if (cmd->background) {
                newInput = malloc(strlen("/dev/null"));
                strcpy(newInput, "/dev/null"); // Background procesess default input to /dev/null
            }
            if (cmd->input) {
                free(newInput);
                newInput = malloc(strlen(cmd->input));
                strcpy(newInput, cmd->input);
            }
            if (newInput) {
                FILE *input = fopen(cmd->input, "r");
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
            /* Redirect output if needed
            -----------------------------*/
            char* newOutput = NULL;
            if (cmd->background) {
                newOutput = malloc(strlen("/dev/null"));
                strcpy(newOutput, "/dev/null");
            }
            if (cmd->output) {
                free(newOutput);
                newOutput = malloc(strlen(cmd->output));
                strcpy(newOutput, cmd->output);
            }
            if (newOutput) {
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
            execvp(cmd->argv[0], cmd->argv);
            printf("%s: no such file or directory\n", cmd->argv[0]);  // execvp only returns if command failed
            exit(1); 
        }
        default: {
            // Pause smallsh for foreground processes 
            if (!cmd->background) {
                int status; // Child process provides new shell status
                waitpid(spawnpid, &status, 0);
                if (status) {
                    // Non-zero status means execvp failed
                    return 1;
                } else {
                    return 0;
                }
                return status;
            } 
            // Don't pause for background processes
            else {
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
