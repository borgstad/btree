#include "btree.h"
#include "hash.h"

// #ifndef STORAGE_H
// #define STORAGE_H
// #endif

Node *ioRead(BlockId id, int maxDegree);
<<<<<<< HEAD
void *ioWrite(Node *node, BlockId id, int maxDegree);
Node *ioAddToCache(id, maxDegree);
Node *ioReplaceCache(id, maxDegree);
void ioFlush(int maxDegree);
=======
void *ioWrite(Node *node, BlockId id, int maxDegree);
>>>>>>> dc0534b0b3fd2f2078464725fc9b584c9c92ac0e
