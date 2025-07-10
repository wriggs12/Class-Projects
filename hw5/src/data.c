#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "data.h"
#include "debug.h"

BLOB *blob_create(char *content, size_t size)
{
    BLOB *bp = malloc(sizeof(BLOB));
    bp->refcnt = 1;
    bp->size = size;
    bp->content = malloc(size + 1);
    bp->prefix = "";

    char *cp = bp->content;
    for(int i = 0; i < (int)size; ++i)
        *cp++ = *(content + i);
    *cp = '\0';

    pthread_mutex_init(&bp->mutex, NULL);

    return bp;
}

BLOB *blob_ref(BLOB *bp, char *why)
{
    pthread_mutex_lock(&bp->mutex);
    
    bp->refcnt++;

    pthread_mutex_unlock(&bp->mutex);
    return bp;
}

void blob_unref(BLOB *bp, char *why)
{
    if (!bp)
        return;
        
    pthread_mutex_lock(&bp->mutex);
    
    bp->refcnt--;

    if (!bp->refcnt)
    {
        free(bp->content);
        free(bp);
    }
    else
        pthread_mutex_unlock(&bp->mutex);
}

int blob_compare(BLOB *bp1, BLOB *bp2)
{
    return strcmp(bp1->content, bp2->content);
}

int blob_hash(BLOB *bp)
{
    int hash = 5381;
    char *c = bp->content;

    while (*c)
    {
        hash = ((hash << 5) + hash) + *c;
        c++;
    }

    return hash;
}

KEY *key_create(BLOB *bp)
{
    KEY *kp = malloc(sizeof(KEY));

    kp->blob = bp;
    kp->hash = blob_hash(bp);

    return kp;
}

void key_dispose(KEY *kp)
{
    blob_unref(kp->blob, "");
    free(kp);
}

int key_compare(KEY *kp1, KEY *kp2)
{
    if (kp1->hash == kp2->hash)
        return 0;
    return 1;
}

VERSION *version_create(TRANSACTION *tp, BLOB *bp)
{
    VERSION *vp = malloc(sizeof(VERSION));
    vp->creator = trans_ref(tp, "");
    vp->blob = bp;
    vp->next = NULL;
    vp->prev = NULL;

    return vp;
}

void version_dispose(VERSION *vp)
{
    blob_unref(vp->blob, "");
    trans_unref(vp->creator, "");

    free(vp); 
}
