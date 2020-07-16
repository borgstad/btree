#include <stdlib.h>
#include "cache.h"
#include "hash.h"
#include "btree.h"
#include "linked_list.h"

static Cache *cache;

void initializeCache(int size)
{
    cache = malloc(sizeof(cache));
    cache->cache = NULL,
    cache->nodeMemStatus = createHashTable(10000),
    cache->lru = NULL,
    cache->nodesInMem = 0;
}

void addItemCache(BlockId id, Node *node)
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

void *getCacheItem(BlockId id, Node *result)
{
    return hashGet(&(cache->nodeMemStatus), id);
}