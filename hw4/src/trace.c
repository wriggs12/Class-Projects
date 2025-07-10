#include "trace.h"

int trace()
{
    Process *proc = get_process();
    if (d_errno == INVAL_DEET_ID)
    {
        error(TRACE);
        return 1;
    }

    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, proc->pid, NULL, &regs);
    void *sp = (void *)regs.rbp;

    while ((long)sp != 0x1)
    {
        long word = ptrace(PTRACE_PEEKDATA, proc->pid, sp + 0x8, NULL);
        fprintf(stdout, "%016lx\t%016lx\n", (long)sp, word);
        sp = (void *)ptrace(PTRACE_PEEKDATA, proc->pid, sp, NULL);
    }

    return 1;
}