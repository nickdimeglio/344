#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "cmd.h"

void ignoreSIGINT() {
    /* Causes the current process to ignore SIGINT (Ctrl-C) 
    */
    sigset_t sigint;
    sigaddset(&sigint, SIGINT);
    struct sigaction ignoreSIGINT = { 
        .sa_handler = SIG_IGN,
        .sa_mask = sigint,
        0,
    };
    sigaction(SIGINT, &ignoreSIGINT, NULL);
}

void ignoreSIGTSTP() {
    /* Causes the current process to ignore SIGTSTP (Ctrl-Z) 
    */
    sigset_t sigint;
    sigaddset(&sigint, SIGTSTP);
    struct sigaction ignoreSIGTSTP = { 
        .sa_handler = SIG_IGN,
        .sa_mask = sigint,
        0,
    };
    sigaction(SIGTSTP, &ignoreSIGTSTP, NULL);
}

