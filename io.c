#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/random.h>
#include "btree.h"
#include "io.h"
#include "hash.h"


/* TODO: fix hack. listSize should be set somewhere else and unallocatedNodes should be dynamically
allocated */
static DiskNode diskNode;


void initialize(int listSize, int hashTableSize)
{
  /* TODO: how to insure singleton class initialization */
  static int i = 0;
  if (i == 0)
    {
  diskNode = (DiskNode) { .n = 0,
			  .listSize = 1000000,
			  .hashTable = createHashTable(10000000)};
    }
  i++;
}
  

Node *diskRead(const Node *nodeIn, Id id)
{
  printf("before fopen\n");

  Node *node = malloc(sizeof(Node));
  FILE *fd = fopen("index.b", "r");
  int filedes = fileno(fd);
  int nodeStructSize = sizeof(Node);
  int keySize = sizeof(int *) * nodeIn -> n;
  int childrenSize = sizeof(Node *) * (nodeIn -> n + 1);
  int nodeOffset = nodeStructSize + keySize + childrenSize;  
  /* int baseOffset = nodeOffset * nodeIn -> levelOrderNr; */
  int baseOffset;
  int returnCode = hashGet(&diskNode.hashTable, id, &baseOffset);
  assert(HASHOK == returnCode);

  /* if (returnCode != HASHOK) */
  /*   { */
  /*     return returnCode; */
  /*   } */
  
  int *keyList = malloc(node -> minDegree * sizeof(int));

  int keyOffset = baseOffset + nodeOffset;
  Node **children = malloc(sizeof(Node) * nodeIn -> minDegree);
  Node *child;
  int childOffset = keyOffset + keySize;
  
  int *key;
  for (int i = 0; i < nodeIn -> n; i++)
    {
      
      key = malloc(sizeof(int));
      pread(filedes, &key, sizeof(int), keyOffset + sizeof(int) * i);

      child = malloc(sizeof(Node));
      pread(filedes, &child, childrenSize, childOffset + nodeStructSize * i);
      children[i] = child;
    }

  printf("after reading: %i %i\n", node -> minDegree, node -> n);
  
  printf("after close\n");

  node -> data = keyList;
  node -> children = children;
  fclose(fd);
  
  return node;
}

void
  diskWrite(Node *node, Id id)
{
  FILE *fd = fopen("index.b", "w+");
  int fildes = fileno(fd);
  int nodeStructSize = sizeof(Node);
  int keySize = sizeof(int *) * node -> n;
  int childrenSize = sizeof(Node *) * (node -> n + 1);
  int idSize = sizeof(int);
  int nodeOffset = nodeStructSize + keySize + childrenSize;
  /* int baseOffset = nodeOffset * node -> levelOrderNr; */
  int baseOffset;
  int returnCode = hashGet(&diskNode.hashTable, id, &baseOffset);
  assert(HASHOK == returnCode);  

  pwrite(fildes, &node, nodeStructSize, baseOffset);    
  pwrite(fildes, &(node -> data), keySize, baseOffset + nodeStructSize);

  Node children[node -> n];
  
  diskNode.n += 1;
  for (int i = 0; i < node -> n; i++)
    {
      children[i] = *(node -> children[i]);
      free(node -> children[i]);
    }
  pwrite(fildes, &(node -> ids), idSize, baseOffset + nodeStructSize + keySize);  
  
  free(node -> data);
  free(node -> children);
  free(node);

  /* fclose(fd);   */
}
