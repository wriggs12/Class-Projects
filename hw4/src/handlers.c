#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "deet.h"
#include "helpers.h"

int HANDLER_RAN = 0;

void sigchld_handler(int sig)
{
    log_signal(sig);
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0)
    {
        if (WIFSTOPPED(status))
            setup_print(pid, PSTATE_STOPPED, 0);
        if (WIFEXITED(status))
            setup_print(pid, PSTATE_DEAD, WEXITSTATUS(status));
        if (WIFSIGNALED(status))
            setup_print(pid, PSTATE_DEAD, WTERMSIG(status));
    }

    HANDLER_RAN = 1;
}

void sigint_handler(int sig)
{
    log_signal(sig);
    quit();
    exit(1);
}

void Signal(int sig, void (*sig_handler)(int))
{
    struct sigaction action;
    struct sigaction old_action;

    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGCHLD);

    action.sa_handler = sig_handler;
    action.sa_mask = sigset;
    action.sa_flags = 0;

    sigaction(sig, &action, &old_action);
}

void block_signal(int sig)
{
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, sig);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
}

void unblock_signal(int sig)
{
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, sig);
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
}

void suspend()
{
    sigset_t sigset;
    sigemptyset(&sigset);
    sigsuspend(&sigset);
}
