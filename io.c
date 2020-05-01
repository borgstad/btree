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

void registerNode(Id nodeId)
{
  hashPut(&diskNode.hashTable, nodeId);  
  diskNode.n += 1; 
}
  

Node *diskRead(const Node *nodeIn, Id id)
{
  printf("before fopen\n");

  Node *node = malloc(sizeof(Node));
  int *data = malloc(sizeof(int) * node -> maxDegree);
  Id *ids = malloc(sizeof(int) * node -> maxDegree);
  
  FILE *fd = fopen("index.b", "r");
  int filedes = fileno(fd);
  int sizeNodeStruct = sizeof(Node);
  int sizeData = sizeof(int *) * nodeIn -> maxDegree;
  int sizeIds = sizeof(int *) * nodeIn -> maxDegree;
  int baseOffset;
  int returnCode = hashGet(&diskNode.hashTable, id, &baseOffset);
  assert(HASHOK == returnCode);  
  baseOffset *= (sizeNodeStruct + sizeData + sizeIds);

  pread(filedes, &node, sizeNodeStruct, baseOffset);
  pread(filedes, &data, sizeData, baseOffset + sizeNodeStruct);
  pread(filedes, &ids, sizeIds, baseOffset + sizeNodeStruct + sizeData);
  
  node -> data;
  node -> ids = ids;
  node -> children = malloc(sizeof(Node) * (2 * node -> maxDegree));
  
  printf("test, n: %i, maxdegree:%i, mindegree: %i\n", node -> n, node -> maxDegree, node -> minDegree);
  /* TODO: why cant i use fclose? */
  /* fclose(fd);   */  
  close(filedes);
  printf("*ASDASD\n");
  return node;
}

void
diskWrite(Node *node, Id id)
{
  FILE *fd = fopen("index.b", "w+");
  int fildes = fileno(fd);
  int sizeNodeStruct = sizeof(Node);
  int sizeData = sizeof(int *) * node -> maxDegree;
  int sizeChildren = sizeof(Node *) * (node -> maxDegree);
  int idSize = sizeof(int) * (node -> maxDegree);
  int baseOffset;
  int returnCode = hashGet(&diskNode.hashTable, id, &baseOffset);
  baseOffset *= (sizeNodeStruct + sizeData + sizeChildren);
  assert(HASHOK == returnCode);

  pwrite(fildes, &node, sizeNodeStruct, baseOffset);    
  pwrite(fildes, &(node -> data), sizeData, baseOffset + sizeNodeStruct);
  pwrite(fildes, &(node -> ids), idSize, baseOffset + sizeNodeStruct + sizeData);
  /* freeNode(node) */
  diskNode.n += 1;
  fclose(fd);
}

void freeNode(Node *node)
{
  for (int i = 0; i < node -> n; i++)
    {
      /* TODO */
    }
  free(node -> data);
  /* free(node -> children); */
  free(node);
}
