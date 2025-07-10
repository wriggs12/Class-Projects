#include "quit.h"

int quit() { 
    block_signal(SIGCHLD);

    Node *node = proc_list;
    while (node)
    {
        if (node->proc->state == PSTATE_DEAD)
        {
            node = node->next;
            continue;
        }

        log_state_change(node->proc->pid, node->proc->state, PSTATE_KILLED, 0);
        update_state(node->proc->pid, PSTATE_KILLED);
        kill(node->proc->pid, SIGKILL);
        node = node->next;
    }

    node = proc_list;
    while (node)
    {
        if (node->proc->state == PSTATE_DEAD)
        {
            node = node->next;
            continue;
        }

        suspend();
        async_print();
        block_signal(SIGCHLD);
    }
    
    return 0;
}