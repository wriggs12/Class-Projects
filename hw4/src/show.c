#include "show.h"

int show()
{
    Process *proc = get_process();
    if (d_errno == INVAL_DEET_ID)
    {
        error(SHOW);
        return 1;
    }

    if (proc)
    {
        print_process(proc);
        return 1;
    }

    Node *cur = proc_list;
    while (cur)
    {
        print_process(cur->proc);
        cur = cur->next;
    }

    return 1;
}
