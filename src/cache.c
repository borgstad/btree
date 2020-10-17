#include <stdlib.h>
#include "include/cache.h"
#include "include/hash.h"
#include "include/btree.h"
#include "include/linked_list.h"
#include "include/storage.h"

/**
 * initializeCache - initalizes the cache
 * @size: size of cache
 *
 * The cache struct consists of a hash table and a linked list. The linked list
 * is a FIFO cache, containing only the blockid of the node.
 * The hash table contains a pointer to the data.
 */
Cache *initializeCache(int size)
{
    Cache *cache = malloc(sizeof(cache));
    cache->nodeMemStatus = createHashTable(10000),
    cache->lru = NULL,
    cache->nodesInMem = 0;
    return cache;
}

/**
 * cacheAdd - adds an element to the cache. Assumes cache is not full
 * 
 * @cache: the cache
 * @id: blockid of the item
 * @node: pointer to the node
 */
void cacheAdd(Cache *cache, BlockId id, Node *node)
{
    if (!cache->lru)
    {
        cache->lru = linkedListInit(id, NULL);
    }
    else
    {
        linkedListAdd(cache->lru, id, NULL);
    }
    hashPut(&(cache->nodeMemStatus), id, node);
    cache->nodesInMem++;
}

/**
 * cacheUpdate - updates a node in the cache
 * 
 * @cache: the cache
 * @id: blockid of the item
 * @node: pointer to the node to be updated
 */
void cacheUpdate(Cache *cache, BlockId id, Node *node)
{
    hashUpdate(&(cache->nodeMemStatus), id, node);
}

/**
 * cacheGet - gets an item from the cache
 * 
 * @cache: the cache
 * @id: blockid of the node to get
 */
void *cacheGet(Cache *cache, BlockId id)
{
    return hashGet(&(cache->nodeMemStatus), id);
}

/**
 * cacheFlush - writes everything in the cache to disk
 * 
 * @cache: the cache
 * @maxDegree: number of keys in each node
 * 
 * Everything in the cache is written to disk. The cache is not emptied.
 * 
 * TODO: empty cache.
 */
void cacheFlush(Cache *cache, int maxDegree)
{
    LinkedList *linkedList = cache->lru;
    while (linkedList)
    {
        BlockId blockId = linkedList->blockId;
        Node *node = hashGet(&(cache->nodeMemStatus), blockId);
        diskWrite(node, blockId, maxDegree);
        linkedList = linkedList->next;
    }
}