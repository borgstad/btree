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
  diskNode = (DiskNode) { .n = 0,
        .listSize = 1000000,
        .hashTable = createHashTable(10000000)};
}

void registerNode(Id nodeId)
{
  hashPut(&diskNode.hashTable, nodeId);  
  diskNode.n += 1; 
}

void
diskWrite(const Node node, Id id, int maxDegree)
{
  FILE *fd = fopen("index.b", "w+");
  int fildes = fileno(fd); 
  int sizeNodeStruct = sizeof(Node);
  int sizeData = sizeof(int) * maxDegree;
  int sizeIds = sizeof(Id) * maxDegree;
  int baseOffset;
  int returnCode = hashGet(&diskNode.hashTable, id, &baseOffset);
  baseOffset *= (sizeNodeStruct + sizeData + sizeIds);
  assert(HASHOK == returnCode);

  pwrite(fildes, &node, sizeNodeStruct, baseOffset);    
  pwrite(fildes, &(node.data), sizeData, baseOffset + sizeNodeStruct);
  pwrite(fildes, &(node.ids), sizeIds, baseOffset + sizeNodeStruct + sizeData);

  diskNode.n += 1;
  fclose(fd);
}


Node diskRead(Id id, int maxDegree)
{

  // int *data = malloc(maxDegree * sizeof(int));
  Node node = (Node) {
    .n = 0,
    .leaf = true,
    .data = malloc(maxDegree * sizeof(int)),
    .ids = malloc(maxDegree * sizeof(Id)),
    .n_ids = 0};
  
  FILE *fd = fopen("index.b", "r");
  int filedes = fileno(fd);
  int sizeNodeStruct = sizeof(Node);
  int sizeData = sizeof(int) * maxDegree;
  int sizeIds = sizeof(Id) * maxDegree;
  int baseOffset;
  int returnCode = hashGet(&diskNode.hashTable, id, &baseOffset);
  assert(HASHOK == returnCode);  
  baseOffset *= (sizeNodeStruct + sizeData + sizeIds);

  int t= (sizeNodeStruct + sizeData + sizeIds);
  pread(filedes, &node, sizeNodeStruct, baseOffset);
  pread(filedes, *node.data, sizeData, baseOffset + sizeNodeStruct);
  pread(filedes, *node.ids, sizeIds, baseOffset + sizeNodeStruct + sizeData);
  
  /* TODO: why cant i use fclose? */
  /* fclose(fd);   */  
  close(filedes);
  return node;
}