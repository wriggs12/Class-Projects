#include "run.h"

int run()
{
    char **argv = malloc(sizeof(char *) * ARG_SIZE);
    char *arg;
    int argc = 1;

    argv[0] = strtok(NULL, DELIMS);
    if (!argv[0])
    {
        log_error("run");
        fprintf(stdout, "?\n");
        return 1;
    }

    while ((arg = strtok(NULL, DELIMS)) != NULL)
    {
        if (argc >= sizeof(argv))
            argv = realloc(argv, sizeof(argv) + ARG_SIZE);
        argv[argc++] = arg;
    }

    if (argc >= sizeof(argv))
            argv = realloc(argv, sizeof(argv) + 1);
    argv[argc] = NULL;

    char *cmd = malloc(sizeof(char) * n);
    char *p = cmd;
    for (int i = 0; i < argc; ++i)
    {
        if (i)
            *p++ = ' ';
        
        for (int j = 0; j < strlen(argv[i]); ++j)
        {
            *p++ = argv[i][j];
        }
    }

    Node *node = proc_list;
    int deet_id = 0;
    while (node)
    {
        if (node->proc->state == PSTATE_DEAD)
        {
            rm_node(node);
            node = node->next;
            continue;
        }

        if (node->proc->deet_id == deet_id)
        {
            deet_id++;
            node = proc_list;
        }
        else
        {
            node = node->next;
        }
    }

    Process *child_proc = malloc(sizeof(Process));
    child_proc->deet_id = deet_id;
    child_proc->trace = 'T';
    child_proc->state = PSTATE_NONE;
    child_proc->cmd = cmd;
    child_proc->exit_status = 0;

    Node *cur_node = malloc(sizeof(Node));
    cur_node->proc = child_proc;
    add_node(cur_node);

    block_signal(SIGCHLD);

    if ((child_proc->pid = fork()) == 0)
    {
        dup2(STDERR_FILENO, STDOUT_FILENO);

        ptrace(PTRACE_TRACEME);
        execvp(argv[0], argv);

        exit(0);
    }

    log_state(child_proc->pid, 0, PSTATE_RUNNING);
    update_state(child_proc->pid, PSTATE_RUNNING);

    suspend();

    return 1;
}
