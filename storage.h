#include "hash.h"
#include "btree.h"

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
void initialize(int listSize, int hashTableSize);
static void freeNode(Node node);