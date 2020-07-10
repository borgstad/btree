#include "hash.h"
#include "btree.h"

#ifndef storage.h
#define storage .h

/* TODO: fix this hacky thing */
typedef struct
{
  int n; // next, in the number of unallocated nodes
  int listSize;
  /* TODO: unallocatedNodes should be a queue, containing the next(n) value of nodes that has been deleted */
  /* int unallocatedNodes[1000000]; // list of unallocated nodes */
  HashTable hashTable;
} DiskNode;

#endif

Node diskRead(Id id, int maxDegree);
void diskWrite(const Node node, Id id, int maxDegree);
void diskClose();
void diskOpen();
void initialize(int listSize, int hashTableSize);
void registerNode(Id nodeId);
static void freeNode(Node node);