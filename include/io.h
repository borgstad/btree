#include "include/btree.h"
#include "include/hash.h"

// #ifndef STORAGE_H
// #define STORAGE_H
// #endif

Node *ioRead(BlockId id, int maxDegree);
void *ioWrite(Node *node, BlockId id, int maxDegree);
void *ioCacheAdd(BlockId id, Node *node);
void *ioCacheReplace(BlockId id, int maxDegree, Node *node);
void ioFlush(int maxDegree);