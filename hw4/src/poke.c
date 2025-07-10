#include "poke.h"

int poke()
{
    Process *proc = get_process();
    if (d_errno == INVAL_DEET_ID)
    {
        error(POKE);
        return 1;
    }

    void *addr = (void *)(strtol(strtok(NULL, DELIMS), NULL, 16));
    long val = strtol(strtok(NULL, DELIMS), NULL, 16);

    ptrace(PTRACE_POKEDATA, proc->pid, addr, val);

    return 1;
}