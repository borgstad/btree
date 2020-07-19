#include "hash.h"
#include "btree.h"

#ifndef CACHE_H
#define CACHE_H

// typedef enum cacheStatus
// {
//     CACHE_HIT,
//     CACHE_MISS
// } cacheStatus;

typedef struct
{
    Node *cache;
    HashTable nodeMemStatus;
    LinkedList *lru;
    int nodesInMem;
} Cache;

#endif

Cache *initializeCache(int size);
void addItemCache(Cache *cache, BlockId id, Node *node);
void *getCacheItem(Cache *cache, BlockId id);