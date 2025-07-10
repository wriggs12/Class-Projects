#include "store.h"
#include "transaction.h"
#include "data.h"

void store_init(void)
{

}

void store_fini(void)
{

}

TRANS_STATUS store_put(TRANSACTION *tp, KEY *key, BLOB *value)
{
    return TRANS_PENDING;
}

TRANS_STATUS store_get(TRANSACTION *tp, KEY *key, BLOB **valuep)
{
    return TRANS_PENDING;
}

void store_show(void)
{

}
