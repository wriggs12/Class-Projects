#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.h"

#define TYPE_OFFSET 0
#define STATUS_OFFSET 1
#define NULL_OFFSET 2
#define SERIAL_OFFSET 4
#define SIZE_OFFSET 8
#define TIMESTAMP_SEC_OFFSET 12
#define TIMESTAMP_NSEC_OFFSET 16

int proto_send_packet(int fd, XACTO_PACKET *pkt, void *data)
{
    char header[sizeof(XACTO_PACKET)];
    memset(header, 0, sizeof(XACTO_PACKET));

    *(header + TYPE_OFFSET) = pkt->type;
    *(header + STATUS_OFFSET) = pkt->status;
    *(header + NULL_OFFSET) = pkt->null;
    *(uint32_t *)(header + SERIAL_OFFSET) = pkt->serial;
    *(uint32_t *)(header + SIZE_OFFSET) = pkt->size;
    *(uint32_t *)(header + TIMESTAMP_SEC_OFFSET) = pkt->timestamp_sec;
    *(uint32_t *)(header + TIMESTAMP_NSEC_OFFSET) = pkt->timestamp_nsec;

    if (write(fd, header, sizeof(XACTO_PACKET)) < 0)
        return -1;

    if (htonl(pkt->size) > 0 && write(fd, (char *)data, htonl(pkt->size)) < 0)
        return -1;

    return 0;
}

int proto_recv_packet(int fd, XACTO_PACKET *pkt, void **datap)
{
    int nbytes;
    char buf[sizeof(XACTO_PACKET)]; 

    if ((nbytes = read(fd, buf, sizeof(XACTO_PACKET))) <= 0)
        return -1;

    pkt->type = (uint8_t)buf[TYPE_OFFSET];
    pkt->status = (uint8_t)buf[STATUS_OFFSET];
    pkt->null = (uint8_t)buf[NULL_OFFSET];
    
    pkt->serial = (*((uint32_t *)(buf + SERIAL_OFFSET)));
    pkt->size = (*((uint32_t *)(buf + SIZE_OFFSET)));
    pkt->timestamp_sec = (*((uint32_t *)(buf + TIMESTAMP_SEC_OFFSET)));
    pkt->timestamp_nsec = (*((uint32_t *)(buf + TIMESTAMP_NSEC_OFFSET)));

    if (htonl(pkt->size) > 0)
    {
        char *data_buf = malloc(htonl(pkt->size) + 1);
        if (read(fd, data_buf, htonl(pkt->size)) < 0)
            return -1;
        data_buf[htonl(pkt->size)] = '\0';
        
        *datap = (void *)data_buf;
    }

    return 0;
}
