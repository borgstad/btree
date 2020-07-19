#include "io.h"
#include "cache.h"
#include "storage.h"
#include <stdlib.h>

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
    if (NULL != node)
    {
        return node;
    }
    // cache miss
    else
    {
        // if cache is not full read from disk and add to cache
        if (maxNodesInMem < cache->nodesInMem)
        {
            return ioAddToCache(id, maxDegree);
        }
        // if cache full, replace the LRU item from the cache
        else
        {
            return ioReplaceCache(id, maxDegree);
        }
    }
}
void *ioWrite(Node *node, BlockId id, int maxDegree)
{
    // if the cache is not full, add the item to the cache
    if (maxNodesInMem < cache->nodesInMem)
    {
        ioAddToCache(id, maxDegree);
    }
    else
    {
        diskWrite(node, id, maxDegree);
    }
}

Node *ioAddToCache(id, maxDegree)
{
    Node *node = diskRead(id, maxDegree);
    addItemCache(cache, id, node);
    cache->nodesInMem++;
    return node;
}

Node *ioReplaceCache(id, maxDegree)
{
    // pop from cache
    LinkedList *firstElement = cache->cache;
    Node *toWrite = firstElement->data;
    BlockId toWriteBlockId = firstElement->blockId;
    deleteLinkedList(cache->cache, toWriteBlockId);
    hashDelete(&(cache->nodeMemStatus), toWriteBlockId);
    diskWrite(toWrite, toWriteBlockId, maxDegree);

    // insert new item
    Node *node = diskRead(id, maxDegree);
    addItemCache(cache, id, node);
    return node;
}
