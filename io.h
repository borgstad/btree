#include "btree.h"
#include "hash.h"

// #ifndef STORAGE_H
// #define STORAGE_H
// #endif

Node *ioRead(BlockId id, int maxDegree);
void *ioWrite(Node *node, BlockId id, int maxDegree);
void *ioAddToCache(BlockId id, Node *node);
void *ioReplaceCache(BlockId id, int maxDegree, Node *node);
void ioFlush(int maxDegree);