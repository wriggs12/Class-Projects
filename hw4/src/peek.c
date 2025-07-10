#include "peek.h"

int peek()
{
    Process *proc = get_process();
    if (d_errno == INVAL_DEET_ID)
    {
        error(PEEK);
        return 1;
    }

    //Handle This
    void *addr = (void *)(strtol(strtok(NULL, DELIMS), NULL, 16));
    long word = ptrace(PTRACE_PEEKDATA, proc->pid, addr, NULL);

    fprintf(stdout, "%016lx\t%016lx\n", (long)addr, word);

    return 1;
}