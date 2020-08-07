#include "include/hash.h"
#include "include/btree.h"

#ifndef STORAGE_H
#define STORAGE_H

typedef struct
{
  LinkedList freeBlock;
  BlockId curBlockId;
} Block;

#endif

Node *diskRead(BlockId id, int maxDegree);
void diskWrite(Node *node, BlockId id, int maxDegree);
void diskClose();
void diskOpen();
void diskInit(int listSize, int hashTableSize);
static void diskFreeNode(Node node);