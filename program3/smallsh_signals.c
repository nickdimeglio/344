#include <signal.h>
#include <stdio.h>
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

void acceptSIGINT() {
    /* Ensure the current process exits on SIGINT (Ctrl-C)
    */
    sigset_t sigint;
    sigaddset(&sigint, SIGINT);
    struct sigaction exitOnSIGINT = {
        .sa_handler = SIG_DFL,
        .sa_mask = sigint,
        0,
    };
    sigaction(SIGINT, &exitOnSIGINT, NULL);
}
