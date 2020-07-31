#include "io.h"
#include "cache.h"
#include "storage.h"
#include <stdlib.h>
#include <stdio.h>

static Cache *cache;
// TODO: set dynamically
static int maxNodesInMem;

void ioInitialize()
{
    cache = initializeCache(100);
    maxNodesInMem = 1024;
}

Node *ioRead(BlockId id, int maxDegree)
{
    Node *node = getCacheItem(cache, id);
    // cache hit
    if (node)
    {
        return node;
    }
    // cache miss
    else
    {
        // if cache is not full read from disk and add to cache
        if (maxNodesInMem > cache->nodesInMem)
        {
            return ioAddToCache(id, maxDegree);
        }
        // if cache full, replace the LRU item from the cache
        else
        {
            printf('replace, ugf\n');
            return ioReplaceCache(id, maxDegree);
        }
    }
}

void *ioWrite(Node *node, BlockId id, int maxDegree)
{
    LinkedList *linkedList = hashGet(&(cache->nodeMemStatus), id);
    hashDelete(&(cache->nodeMemStatus), id);
    deleteLinkedList(cache->lru, id);

    hashPut(&(cache->nodeMemStatus), id, node);
    addLinkedList(cache->lru, id, node);
}

Node *ioAddToCache(id, maxDegree)
{
    Node *node = diskRead(id, maxDegree);
    addItemCache(cache, id, node);
    return node;
}

Node *ioReplaceCache(id, maxDegree)
{
    // pop from cache
    LinkedList *firstElement = cache->lru;
    Node *toWrite = firstElement->data;
    BlockId toWriteBlockId = firstElement->blockId;
    deleteLinkedList(cache->lru, toWriteBlockId);
    hashDelete(&(cache->nodeMemStatus), toWriteBlockId);
    diskWrite(toWrite, toWriteBlockId, maxDegree);

    // insert new item
    Node *node = diskRead(id, maxDegree);
    addItemCache(cache, id, node);
    return node;
}

void ioFlush(int maxDegree)
{
    cacheFlush(cache, maxDegree);
}