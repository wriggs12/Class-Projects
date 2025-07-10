#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "server.h"
#include "transaction.h"
#include "data.h"
#include "store.h"
#include "protocol.h"
#include "debug.h"

CLIENT_REGISTRY *client_registry;

void send_reply(int fd, uint32_t serial, int status);
void send_value(int fd, uint32_t serial, BLOB *bp);

void *xacto_client_service(void *arg)
{
    int fd = *(int *)arg;
    free(arg);

    pthread_detach(pthread_self());
    creg_register(client_registry, fd);

    int serial;
    TRANSACTION *tp = trans_create();
    XACTO_PACKET *pkt = malloc(sizeof(XACTO_PACKET));
    void **datap = malloc(sizeof(void *));

    while (1)
    {
        if (proto_recv_packet(fd, pkt, datap) == -1)
        {
            trans_abort(tp);
            free(pkt);
            free(datap);
            return NULL;
        }
        serial = pkt->serial;

        if (pkt->type == XACTO_PUT_PKT)
        {
            proto_recv_packet(fd, pkt, datap);

            BLOB *key_data = blob_create(*(char **)datap, strlen(*(char **)datap));
            KEY *key = key_create(key_data);

            proto_recv_packet(fd, pkt, datap);

            BLOB *value = blob_create(*(char **)datap, strlen(*(char **)datap));

            store_put(tp, key, value);
            send_reply(fd, serial, 0);
        }
        else if (pkt->type == XACTO_GET_PKT)
        {
            proto_recv_packet(fd, pkt, datap);

            BLOB *key_data = blob_create(*(char **)datap, strlen(*(char **)datap));
            KEY *key = key_create(key_data);

            BLOB **valuep = malloc(sizeof(BLOB *));

            store_get(tp, key, valuep);
            send_reply(fd, serial, 0);
            send_value(fd, serial, *valuep);
        }
        else if (pkt->type == XACTO_COMMIT_PKT)
        {
            trans_commit(tp);
            send_reply(fd, serial, 1);
            free(pkt);
            free(datap);
            return NULL;
        }
    }

    return NULL;
}

void send_reply(int fd, uint32_t serial, int status)
{
    XACTO_PACKET *pkt = malloc(sizeof(XACTO_PACKET));
    pkt->type = XACTO_REPLY_PKT;
    pkt->status = status;
    pkt->null = 0;
    pkt->serial = serial;
    pkt->size = 0;
    pkt->timestamp_sec = 0;
    pkt->timestamp_nsec = 0;

    proto_send_packet(fd, pkt, NULL);
}

void send_value(int fd, uint32_t serial, BLOB *bp)
{
    XACTO_PACKET *pkt = malloc(sizeof(XACTO_PACKET));
    pkt->type = XACTO_VALUE_PKT;
    pkt->status = 0;
    pkt->serial = serial;
    pkt->timestamp_sec = 0;
    pkt->timestamp_nsec = 0;

    if (bp)
    {
        pkt->null = 0;
        pkt->size = htonl(strlen(bp->content));
        proto_send_packet(fd, pkt, bp->content);
    }
    else
    {
        pkt->null = 1;
        pkt->size = 0;
        proto_send_packet(fd, pkt, NULL);
    }
}
