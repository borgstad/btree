#include "include/io.h"
#include "include/cache.h"
#include "include/storage.h"
#include <stdlib.h>

static Cache *cache;
// TODO: set dynamically
static int maxNodesInMem;

void ioInitialize()
{
    initializeStorage();
    cache = initializeCache(100);
    maxNodesInMem = 1024;
}

Node *ioRead(BlockId id, int maxDegree)
{
    Node *node = cacheGet(cache, id);
    // cache hit
    if (node)
    {
        return node;
    }
    // cache miss
    else
    {
        // if cache is not full read from disk and add to cache
        if (cache->nodesInMem < maxNodesInMem)
        {
            Node *node = diskRead(id, maxDegree);
            ioCacheAdd(id, node);
            return node;
        }
        // if cache is full, replace first LRU item from the cache
        else
        {
            printf("ioRead cache full\n");
            // return ioCacheReplace(id, maxDegree);
        }
    }
}

void *ioWrite(Node *node, BlockId id, int maxDegree)
{
    // if the cache is not full, either update or add entry
    if (cache->nodesInMem < maxNodesInMem)
    {
        // node is in cache
        Node *oldNode = cacheGet(cache, id);
        if (oldNode)
        {
            // printf("iowrite curval \t id: %i\t val: %i\n", id, oldNode->data[0]);
            cacheUpdate(cache, id, node);
            Node *readRode = ioRead(id, maxDegree);
        }
        else
        {
            ioCacheAdd(id, node);
            return node;
        }
    }
    // the cache is full
    else
    {
        ioCacheReplace(id, maxDegree, node);
    }
}

void *ioCacheAdd(BlockId id, Node *node)
{
    cacheAdd(cache, id, node);
    cache->nodesInMem++;
}

void *ioCacheReplace(BlockId id, int maxDegree, Node *node)
{
    BlockId toWriteBlockId = cache->lru->blockId;
    Node *toWrite = cacheGet(cache, toWriteBlockId);
    diskWrite(toWrite, toWriteBlockId, maxDegree);
    linkedListDelete(cache->lru, toWriteBlockId);
    hashDelete(&(cache->nodeMemStatus), toWriteBlockId);
    cache->nodesInMem--;

    // insert new item cache->lru;
    ioCacheAdd(id, node);
}

void ioFlush(int maxDegree)
{
    cacheFlush(cache, maxDegree);
}
