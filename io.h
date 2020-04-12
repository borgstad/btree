#include "hash.h"
#include "btree.h"



/* TODO: fix this hacky thing */
typedef struct
{
  int n; // next, in the number of unallocated nodes
  int listSize;
  int unallocatedNodes[1000000]; // list of unallocated nodes
  HashTable hashTable;
} DiskNode;


Node *diskRead(const Node *node, Id id);
void diskWrite(Node *node);


/* auxiliary functions */

int getDiskOffset(Id id);
