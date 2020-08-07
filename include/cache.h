#include "include/hash.h"
#include "include/btree.h"

#ifndef CACHE_H
#define CACHE_H

// typedef enum cacheStatus
// {
//     CACHE_HIT,
//     CACHE_MISS
// } cacheStatus;

typedef struct
{
    HashTable nodeMemStatus;
    LinkedList *lru;
    int nodesInMem;
} Cache;

#endif

Cache *initializeCache(int size);
void cacheAdd(Cache *cache, BlockId id, Node *node);
void *cacheGet(Cache *cache, BlockId id);
void cacheFlush(Cache *cache, int maxDegree);
void cacheUpdate(Cache *cache, BlockId id, Node *node);
