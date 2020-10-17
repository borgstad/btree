#include "include/io.h"
#include "include/cache.h"
#include "include/storage.h"
#include <stdlib.h>

static Cache *cache;
// TODO: set dynamically
static int maxNodesInMem;

/**
 * ioInitialize - initialization of IO module
 *
 * Entry function for initializing IO module, used by the btree. This module
 * initializes a storage- and cache module. The storage module supports disk reads,
 * the cache is a simple in-memory FIFO, whose size is statically set in this module.
 * The IO operations are performed on individual nodes, and the result is always
 * materialized in the in-memory cache. It is only if the cache is fully, that the
 * oldest element is written to disk, and removed from the cache.
 */
void ioInitialize()
{
    initializeStorage();
    cache = initializeCache(100);
    maxNodesInMem = 1024;
}

/**
 * ioRead - main read abstraction used by the btree for reading nodes
 * 
 * @id: id to read
 * @maxDegree: max keys in node.
 *
 * The id is assumed to be unique, and first the cache is searched for the item.
 * If the item is in the cache, it is returned. If not, then it is read from
 * disk and insert into the cache.
 */
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

/**
 * ioWrite - main write abstraction used by the btree for writing nodes
 * 
 * @node: the node to write
 * @id: id of node to write
 * @maxDegree: max keys in node
 * 
 * Adds the node to cache. If the node is already there, the cache is updated. If the
 * cache is full, replace the oldest element, and insert the newest.
 */
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

/**
 * ioCacheAdd - adds an element to the cache. Assumes the cache is not full
 * 
 * @id: id of node
 * @node: pointer to node
 *
 */
void *ioCacheAdd(BlockId id, Node *node)
{
    cacheAdd(cache, id, node);
    cache->nodesInMem++;
}

/**
 * ioCacheReplace - replaces the oldest element from the cahce, and inserts new element
 * 
 * @id: id of node to insert
 * @maxDegree: max number of keys for node
 * @node: node to insert
 */
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

/** ioFlush - wrapper function to flush the cache, i.e., write everything to disk
 * 
 * @maxDegree: max number of keys for nodes
 */
void ioFlush(int maxDegree)
{
    cacheFlush(cache, maxDegree);
}
