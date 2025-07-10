#include "stop.h"

int stop()
{
    Process *proc = get_process();
    if (d_errno == INVAL_DEET_ID || proc->state != PSTATE_RUNNING)
    {
        error(STOP);
        return 1;
    }

    log_state(proc->pid, 0, PSTATE_STOPPING);
    update_state(proc->pid, PSTATE_STOPPING);
    kill(proc->pid, SIGSTOP);

    return 1;
}
