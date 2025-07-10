#include "wait.h"

int wait_deet()
{
    Process *proc = get_process();
    if (d_errno == INVAL_DEET_ID)
    {
        error(WAIT);
        return 1;
    }

    char *state = strtok(NULL, DELIMS);
    if (!state)
        state = states[PSTATE_DEAD];
    else
    {
        int valid_state = 0;
        for (int i = 0; i < NUM_STATES; ++i)
            if (!strcmp(states[i], state))
            {
                valid_state = 1;
                break;
            }

        if (!valid_state)
        {
            error(WAIT);
            return 1;
        }
    }

    while (strcmp(states[proc->state], state));

    return 1;
}
