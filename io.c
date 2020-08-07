#include "io.h"
#include "cache.h"
#include "storage.h"
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
        if (cache->nodesInMem < maxNodesInMem)
        {
            Node *node = diskRead(id, maxDegree);
            ioAddToCache(id, node);
            return node;
        }
        // if cache is full, replace first LRU item from the cache
        else
        {
            printf("ioRead cache full\n");
            // return ioReplaceCache(id, maxDegree);
        }
    }
}

void *ioWrite(Node *node, BlockId id, int maxDegree)
{
    // if the cache is not full, either update or add entry
    if (cache->nodesInMem < maxNodesInMem)
    {
        // node is in cache
        Node *oldNode = getCacheItem(cache, id);
        if (oldNode)
        {
            // printf("iowrite curval \t id: %i\t val: %i\n", id, oldNode->data[0]);
            cacheUpdateItem(cache, id, node);
            Node *readRode = ioRead(id, maxDegree);
        }
        else
        {
            ioAddToCache(id, node);
            return node;
        }
    }
    // the cache is full
    else
    {
        ioReplaceCache(id, maxDegree, node);
    }
}

void *ioAddToCache(BlockId id, Node *node)
{
    addItemCache(cache, id, node);
    cache->nodesInMem++;
}

void *ioReplaceCache(BlockId id, int maxDegree, Node *node)
{
    BlockId toWriteBlockId = cache->lru->blockId;
    Node *toWrite = getCacheItem(cache, toWriteBlockId);
    diskWrite(toWrite, toWriteBlockId, maxDegree);
    deleteLinkedList(cache->lru, toWriteBlockId);
    hashDelete(&(cache->nodeMemStatus), toWriteBlockId);
    cache->nodesInMem--;

    // insert new item cache->lru;
    ioAddToCache(id, node);
}

void ioFlush(int maxDegree)
{
    cacheFlush(cache, maxDegree);
}
