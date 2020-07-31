#include <stdlib.h>
#include "cache.h"
#include "hash.h"
#include "btree.h"
#include "linked_list.h"
#include "storage.h"

Cache *initializeCache(int size)
{
    Cache *cache = malloc(sizeof(cache));
    cache->nodeMemStatus = createHashTable(10000),
    cache->lru = NULL,
    cache->nodesInMem = 0;
    return cache;
}

void addItemCache(Cache *cache, BlockId id, Node *node)
{
    if (!cache->lru)
    {
        cache->lru = initializeLinkedList(id, node);
    }
    else
    {
        addLinkedList(cache->lru, id, node);
    }
    hashPut(&(cache->nodeMemStatus), id, node);
    cache->nodesInMem++;
}

void *getCacheItem(Cache *cache, BlockId id)
{
    return hashGet(&(cache->nodeMemStatus), id);
}

void cacheFlush(Cache *cache, int maxDegree)
{
    LinkedList *linkedList = cache->lru;
    int i = 0;
    while (linkedList)
    {
        diskWrite(linkedList->data, linkedList->blockId, maxDegree);
        linkedList = linkedList->next;
    }
}