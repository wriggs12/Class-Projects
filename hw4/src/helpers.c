#include "helpers.h"

Node *proc_list = NULL;
int d_errno = 0;
char *input;
size_t n;
State_Info *buffer = NULL;

char *states[] = {
    "none",
    "running",
    "stopping",
    "stopped",
    "continuing",
    "killed",
    "dead"
};

char *cmds[] = {
    HELP,
    QUIT,
    SHOW,
    RUN,
    STOP,
    CONTINUE,
    RELEASE,
    WAIT,
    KILL,
    PEEK,
    POKE,
    TRACE
};

int (*cmd_func[]) () = {
    &help,
    &quit,
    &show,
    &run,
    &stop,
    &cont,
    &release,
    &wait_deet,
    &kill_deet,
    &peek,
    &poke,
    &trace
};

extern int silent_logging;

void shell(int prompt)
{
    Signal(SIGCHLD, &sigchld_handler);
    Signal(SIGINT, &sigint_handler);

    log_startup();
    int status = 1;

    while(status)
    {
        async_print();
        log_prompt();
        
        unblock_signal(SIGCHLD);
        
        if (prompt)
            fprintf(stdout, PROMPT);
        fflush(stdout);
        
        // unblock_signal(SIGCHLD);
    
        status = handle_input();
        d_errno = NONE;
    }

    free(input);
    log_shutdown();
}

void async_print()
{
    block_signal(SIGCHLD);
    if (HANDLER_RAN)
    {
        while (buffer)
        {
            update_state(buffer->pid, buffer->state);

            State_Info *temp = buffer;
            buffer = buffer->next;

            free(temp);
        }

        HANDLER_RAN = 0;
    }
    unblock_signal(SIGCHLD);
}

int handle_input()
{
    int rv;
    while (HANDLER_RAN || (rv = getline(&input, &n, stdin)) == -1)
    {
        if (HANDLER_RAN)
        {
            async_print();
            continue;
        }
        
        if (errno == EINTR)
        {
            clearerr(stdin);
            return 1;
        }

        return quit();
    }
    
    log_input(input);
    char *cmd = strtok(input, DELIMS);
    
    if (!cmd)
        return 1;

    for (int i = 0; i < NUM_CMDS; ++i)
        if (strcmp(cmd, cmds[i]) == 0)
            return (*cmd_func[i]) ();

    log_error(input);
    fprintf(stdout, "?\n");
    return 1;
}

void print_process(Process *p)
{
    fprintf(stdout, "%d\t%d\t%c\t%s\t", 
            p->deet_id, 
            p->pid,
            p->trace,
            states[p->state]
    );

    if (p->state == PSTATE_DEAD)
        fprintf(stdout, "0x%x", p->exit_status);
    
    fprintf(stdout, "\t%s\n", p->cmd);
}

void log_state(pid_t pid, int exit_status, PSTATE state)
{
    Node *node = proc_list;
    while (node->proc->pid != pid)
        node = node->next;

    log_state_change(pid, node->proc->state, state, 0);
    node->proc->state = state;
    node->proc->exit_status = exit_status;
}

void update_state(int pid, PSTATE new_state)
{
    Node *node = proc_list;
    while (node->proc->pid != pid)
        node = node->next;

    node->proc->state = new_state;
    print_process(node->proc);
}

void rm_node(Node *node)
{
    if (proc_list == node)
    {
        if (proc_list)
            proc_list->prev = NULL;
        proc_list = node->next;
        return;
    }

    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
}

void add_node(Node *node)
{
    if (!proc_list)
    {
        proc_list = node;
        return;
    }

    Node *end = proc_list;
    while (end->proc->deet_id < node->proc->deet_id && end->next)
        end = end->next;
    
    if (end->proc->deet_id > node->proc->deet_id)
    {
        if (end->prev)
        {
            end->prev->next = node;
            node->prev = end->prev;
        }
        else
            proc_list = node;
        
        node->next = end;
        end->prev = node;
    }
    else
    {
        end->next = node;
        node->prev = end;
    }
}

Process *get_process()
{
    char *deet_id = strtok(NULL, DELIMS);
    if (!deet_id)
        return NULL;
    
    Node *node = proc_list;
    while (node && node->proc->deet_id != strtol(deet_id, NULL, 10))
        node = node->next;

    if (!node)
    {   d_errno = INVAL_DEET_ID;
        return NULL;
    }
    
    return node->proc;
}

void error(char *cmd)
{
    log_error(cmd);
    fprintf(stdout, "?\n");
}

void setup_print(pid_t pid, PSTATE state, int exit_status)
{
    State_Info *cur = malloc(sizeof(State_Info));
    cur->pid = pid;
    cur->state = state;
    cur->exit_status = exit_status;
    cur->next = NULL;

    log_state(pid, exit_status, state);

    if (!buffer)
    {
        buffer = cur;
        return;
    }

    State_Info *b = buffer;
    while (b->next)
        b = b->next;

    b->next = cur;
}