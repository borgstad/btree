#include "hash.h"
#include "btree.h"

#ifndef storage.h
#define storage .h

typedef struct
{
  LinkedList freeBlock;
  BlockId curBlockId;
} Block;

#endif

Node diskRead(BlockId id, int maxDegree);
void diskWrite(const Node node, BlockId id, int maxDegree);
void diskClose();
void diskOpen();
void initialize(int listSize, int hashTableSize);
static void freeNode(Node node);