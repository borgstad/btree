#include <stdlib.h>
#include "cache.h"
#include "hash.h"
#include "btree.h"
#include "linked_list.h"

Cache *initializeCache(int size)
{
    Cache *cache = malloc(sizeof(cache));
    cache->cache = NULL,
    cache->nodeMemStatus = createHashTable(10000),
    cache->lru = NULL,
    cache->nodesInMem = 0;
    return cache;
}

void addItemCache(Cache *cache, BlockId id, Node *node)
{
    if (!cache->cache)
    {
        // printf("cache: %i\n", node->n);
        cache->cache = initializeLinkedList(id, node);
    }
    else
    {
        addLinkedList(cache->cache, id, node);
    }
    hashPut(&(cache->nodeMemStatus), id, node);
}

void *getCacheItem(Cache *cache, BlockId id)
{
    return hashGet(&(cache->nodeMemStatus), id);
}