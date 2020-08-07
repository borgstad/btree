#include <stdlib.h>
#include "include/cache.h"
#include "include/hash.h"
#include "include/btree.h"
#include "include/linked_list.h"
#include "include/storage.h"

Cache *initializeCache(int size)
{
    Cache *cache = malloc(sizeof(cache));
    cache->nodeMemStatus = createHashTable(10000),
    cache->lru = NULL,
    cache->nodesInMem = 0;
    return cache;
}

void cacheAdd(Cache *cache, BlockId id, Node *node)
{
    if (!cache->lru)
    {
        cache->lru = linkedListInit(id, NULL);
    }
    else
    {
        linkedListAdd(cache->lru, id, NULL);
    }
    hashPut(&(cache->nodeMemStatus), id, node);
    cache->nodesInMem++;
}

void cacheUpdate(Cache *cache, BlockId id, Node *node)
{
    hashUpdate(&(cache->nodeMemStatus), id, node);
}

void *cacheGet(Cache *cache, BlockId id)
{
    return hashGet(&(cache->nodeMemStatus), id);
}

void cacheFlush(Cache *cache, int maxDegree)
{
    LinkedList *linkedList = cache->lru;
    while (linkedList)
    {
        // printf("%i\n", linkedList->blockId);
        BlockId blockId = linkedList->blockId;
        Node *node = hashGet(&(cache->nodeMemStatus), blockId);
        diskWrite(node, blockId, maxDegree);
        linkedList = linkedList->next;
    }
}