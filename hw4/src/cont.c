#include "cont.h"

int cont()
{
    block_signal(SIGCHLD);

    Process *proc = get_process();
    if (d_errno == INVAL_DEET_ID || proc->state != PSTATE_STOPPED)
    {
        error(CONTINUE);
        return 1;
    }

    if (proc->trace == 'U')
    {
        kill(proc->pid, SIGCONT);
        
        log_state(proc->pid, 0, PSTATE_CONTINUING);
        update_state(proc->pid, PSTATE_CONTINUING);
    }
    else
        ptrace(PTRACE_CONT, proc->pid, NULL, SIGCONT);

    log_state(proc->pid, 0, PSTATE_RUNNING);
    update_state(proc->pid, PSTATE_RUNNING);

    return 1;
}
