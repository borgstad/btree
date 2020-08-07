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
    // printf("read\n");
    // cache hit
    if (node)
    {
        // printf("read cached node %i\n", node->data[0]);
        return node;
    }
    // cache miss
    else
    {
        printf("IO: Cache miss\n");
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
            printf("TODO\n");
            // return ioReplaceCache(id, maxDegree);
        }
    }
}

void *ioWrite(Node *node, BlockId id, int maxDegree)
{
    // if the cache is not full, either update or add entry
    // printf("write\n");
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
    // TODO
    else
    {
        printf("TODO\n");
        diskWrite(node, id, maxDegree);
    }
}

void *ioAddToCache(BlockId id, Node *node)
{
    addItemCache(cache, id, node);
    cache->nodesInMem++;
}

void *ioReplaceCache(BlockId id, int maxDegree)
{
    // TODO: not implemented
    // pop from cache
    printf("TODO, replace cache\n");
    LinkedList *firstElement = cache->lru;
    Node *toWrite = firstElement->data;
    BlockId toWriteBlockId = firstElement->blockId;
    deleteLinkedList(cache->lru, toWriteBlockId);
    hashDelete(&(cache->nodeMemStatus), toWriteBlockId);
    diskWrite(toWrite, toWriteBlockId, maxDegree);

    // insert new item cache->lru;
    Node *node = diskRead(id, maxDegree);
    addItemCache(cache, id, node);
    return node;
}

void ioFlush(int maxDegree)
{
    cacheFlush(cache, maxDegree);
}
