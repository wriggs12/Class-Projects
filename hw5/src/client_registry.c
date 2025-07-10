#include <stdlib.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdio.h>

#include "client_registry.h"

struct client_registry {
    int n;
    fd_set client_fd;
    sem_t creg_mutex;
    sem_t count_mutex;
};

CLIENT_REGISTRY *creg_init()
{ 
    CLIENT_REGISTRY *creg = malloc(sizeof(CLIENT_REGISTRY));

    creg->n = 0;
    FD_ZERO(&creg->client_fd);
    sem_init(&creg->creg_mutex, 0, 1);
    sem_init(&creg->count_mutex, 0, 1);

    return creg;
}

void creg_fini(CLIENT_REGISTRY *cr)
{
    sem_destroy(&cr->creg_mutex);
    free(cr);
}

int creg_register(CLIENT_REGISTRY *cr, int fd)
{
    sem_wait(&cr->creg_mutex);
    FD_SET(fd, &cr->client_fd);

    cr->n++;
    if (cr->n == 1)
        sem_wait(&cr->count_mutex);

    sem_post(&cr->creg_mutex);

    return 0;
}

int creg_unregister(CLIENT_REGISTRY *cr, int fd)
{
    sem_wait(&cr->creg_mutex);
    FD_CLR(fd, &cr->client_fd);

    cr->n--;
    if (cr->n == 0)
        sem_post(&cr->count_mutex);

    sem_post(&cr->creg_mutex);

    return 0;
}

void creg_wait_for_empty(CLIENT_REGISTRY *cr)
{
    int n = 1;
    while (n)
    {
        sem_wait(&cr->creg_mutex);
            n = cr->n;
        sem_post(&cr->creg_mutex);
    }
}

void creg_shutdown_all(CLIENT_REGISTRY *cr)
{
    sem_wait(&cr->creg_mutex);

    for (int i = 4; i < FD_SETSIZE; ++i)
        if (FD_ISSET(i, &cr->client_fd))
        {
            shutdown(i, SHUT_RDWR); 
            cr->n--;
        }

    sem_post(&cr->creg_mutex);
}
