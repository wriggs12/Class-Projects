#include "release.h"

int release()
{
    Process *proc = get_process();
    if (d_errno == INVAL_DEET_ID)
    {
        error(RELEASE);
        return 1;
    }

    block_signal(SIGCHLD);
    proc->trace = 'U';
    log_state(proc->pid, 0, PSTATE_RUNNING);
    update_state(proc->pid, PSTATE_RUNNING);
    unblock_signal(SIGCHLD);
    
    ptrace(PTRACE_DETACH, proc->pid, NULL, NULL);

    return 1;
}