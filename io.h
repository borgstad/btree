#include "btree.h"
#include "hash.h"

// #ifndef STORAGE_H
// #define STORAGE_H
// #endif

Node *ioRead(BlockId id, int maxDegree);
void *ioWrite(Node *node, BlockId id, int maxDegree);
Node *ioAddToCache(id, maxDegree);
Node *ioReplaceCache(id, maxDegree);
void ioFlush(int maxDegree);