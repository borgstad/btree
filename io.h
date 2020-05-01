#include "hash.h"
#include "btree.h"

#ifndef IO_H
#define IO_H

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

Node *diskRead(const Node *node, Id id);
void diskWrite(Node *node, Id id);
void initialize(int listSize, int hashTableSize);
void registerNode(Id nodeId);

/* auxiliary functions */

int getDiskOffset(Id id);
