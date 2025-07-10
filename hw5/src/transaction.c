// #include <stdlib.h>
// #include <stdio.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <sys/socket.h>
// #include <sys/select.h>

// #include "transaction.h"
// #include "store.h"

// void trans_init(void)
// {
//     trans_list.id = -1;
//     trans_list.refcnt = -1;
//     trans_list.next = &trans_list;
//     trans_list.prev = &trans_list;
// }

// void trans_fini(void)
// {
//     TRANSACTION *tp = trans_list.next;
//     DEPENDENCY *dp;

//     while (tp != &trans_list)
//     {
//         dp = tp->depends;
//         while (dp)
//         {
//             DEPENDENCY *temp = dp;
//             free(temp);
//             dp = dp->next;
//         }

//         tp = tp->next;
//         free(tp->prev);
//     }
// }

// TRANSACTION *trans_create(void)
// {
//     TRANSACTION *tp = malloc(sizeof(TRANSACTION));

//     tp->id = trans_list.prev->id + 1;
//     tp->refcnt = 1;
//     tp->status = TRANS_PENDING;
//     tp->waitcnt = 0;
//     tp->depends = NULL;

//     tp->next = &trans_list;
//     tp->prev = trans_list.prev;

//     trans_list.prev->next = tp;
//     trans_list.prev = tp;

//     sem_init(&tp->sem, 0, 1);
//     pthread_mutex_init(&tp->mutex, NULL);

//     return tp;
// }

// TRANSACTION *trans_ref(TRANSACTION *tp, char *why)
// {
//     pthread_mutex_lock(&tp->mutex);
    
//     tp->refcnt++;

//     pthread_mutex_unlock(&tp->mutex);
//     return tp;
// }

// void trans_unref(TRANSACTION *tp, char *why)
// {
//     pthread_mutex_lock(&tp->mutex);

//     tp->refcnt--;

//     if (!tp->refcnt)
//     {
//         free(tp->depends);
//         free(tp);
//     }
//     else
//         pthread_mutex_unlock(&tp->mutex);
// }

// void trans_add_dependency(TRANSACTION *tp, TRANSACTION *dtp)
// {
//     DEPENDENCY *dp = tp->depends;

//     while (dp)
//         dp = dp->next;

//     dp = malloc(sizeof(DEPENDENCY));
//     dp->trans = dtp;
//     dp->next = NULL;
// }

// TRANS_STATUS trans_commit(TRANSACTION *tp)
// {
//     while (tp->depends)
//     {
//         while (trans_get_status(tp->depends->trans) == TRANS_PENDING);

//         if (trans_get_status(tp->depends->trans) == TRANS_ABORTED)
//             return TRANS_ABORTED;

//         tp->depends = tp->depends->next;
//     }

//     tp->status = TRANS_COMMITTED;

//     return TRANS_COMMITTED;
// }

// TRANS_STATUS trans_abort(TRANSACTION *tp)
// {
//     if (tp->status == TRANS_ABORTED)
//         return TRANS_ABORTED;
//     if (tp->status == TRANS_COMMITTED)
//         exit(1);

//     tp->status = TRANS_ABORTED;
//     return TRANS_ABORTED;
// }

// TRANS_STATUS trans_get_status(TRANSACTION *tp)
// {
//     return tp->status;
// }

// void trans_show(TRANSACTION *tp)
// {

// }

// void trans_show_all(void)
// {

// }
