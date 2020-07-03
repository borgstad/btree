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
  diskNode = (DiskNode){.n = 0,
                        .listSize = 1000000,
                        .hashTable = createHashTable(10000000)};
}

void registerNode(Id nodeId)
{
  hashPut(&diskNode.hashTable, nodeId);
  diskNode.n += 1;
}

Node diskRead(Id id, int maxDegree)
{
  int *data = malloc(maxDegree * sizeof(int));
  Id *ids = malloc((maxDegree + 1) * sizeof(Id));
  Node node = (Node){
      .n = 0,
      .leaf = true,
      .data = NULL,
      .ids = NULL,
      .n_ids = 0};

  FILE *fd = fopen("index.b", "r");
  int filedes = fileno(fd);
  int sizeNodeStruct = sizeof(Node);
  int sizeData = sizeof(int) * maxDegree;
  int sizeIds = sizeof(Id) * (maxDegree + 1);
  int baseOffset;
  int returnCode = hashGet(&diskNode.hashTable, id, &baseOffset);
  assert(HASHOK == returnCode);
  baseOffset *= (sizeNodeStruct + sizeData + sizeIds);
  // printf("read baseOffset: %i, %lu\n", baseOffset, id);
  // printf("re:\t %i\t %i\t %i\n", baseOffset, baseOffset + sizeNodeStruct, baseOffset + sizeNodeStruct + sizeData);

  pread(filedes, &node, sizeNodeStruct, baseOffset);
  node.data = data;
  node.ids = ids;
  pread(filedes, (int *)node.data, sizeData, baseOffset + sizeNodeStruct);
  pread(filedes, (Id *)node.ids, sizeIds, baseOffset + sizeNodeStruct + sizeData);

  // for (int i = 0; i < maxDegree; i++)
  //   {
  //     node.ids[i] = i;
  //   }
  fclose(fd);
  return node;
}

void diskWrite(const Node node, Id id, int maxDegree)
{
  FILE *fd = fopen("index.b", "r+");
  int fildes = fileno(fd);
  int sizeNodeStruct = sizeof(Node);
  int sizeData = sizeof(int) * maxDegree;
  int sizeIds = sizeof(Id) * (maxDegree + 1);
  int baseOffset;
  int returnCode = hashGet(&diskNode.hashTable, id, &baseOffset);
  baseOffset *= (sizeNodeStruct + sizeData + sizeIds);
  assert(HASHOK == returnCode);
  // printf("write: %i\t %i\t %i\n", baseOffset, baseOffset + sizeNodeStruct, baseOffset + sizeNodeStruct + sizeData);
  // printf("write baseOffset: %i, %ld\n", baseOffset, id);
  // for (int i = 0; i < maxDegree; i++)
  // {
  //   printf("%lu\n", node.ids[i]);
  // }
  int status;
  // printf("sizeids: %i\n", sizeIds);
  status = pwrite(fildes, &node, sizeNodeStruct, baseOffset);
  status = pwrite(fildes, node.data, sizeData, baseOffset + sizeNodeStruct);
  status = pwrite(fildes, node.ids, sizeIds, baseOffset + sizeNodeStruct + sizeData);
  diskNode.n += 1;
  fclose(fd);
}

static void
freeNode(Node node)
{
  free(node.data);
  free(node.ids);
}