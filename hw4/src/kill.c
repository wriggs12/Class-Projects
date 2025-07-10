#include "kill.h"

int kill_deet()
{
    Process *proc = get_process();
    if (d_errno == INVAL_DEET_ID || proc->state == PSTATE_NONE || proc->state == PSTATE_KILLED || proc->state == PSTATE_DEAD)
    {
        error(KILL);
        return 1;
    }
    
    update_state(proc->pid, PSTATE_KILLED);
    kill(proc->pid, SIGKILL);

    return 1;
}

