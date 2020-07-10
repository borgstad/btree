#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/random.h>
#include "btree.h"
#include "storage.h"
#include "linked_list.h"
#include "hash.h"

/* TODO: fix hack. listSize should be set somewhere else and unallocatedNodes should be dynamically
allocated */
static Block block;
static FILE *fd;
static int fildes;

void initializeStorage()
{
  block = (Block){
      initializeLinkedList(0),
      0};
  fd = fopen("index.b", "w+");
  fildes = fileno(fd);
}

BlockId getNewBlockId()
{
  block.curBlockId++;
  return block.curBlockId;
}

Node diskRead(BlockId id, int maxDegree)
{
  int *data = malloc(maxDegree * sizeof(int));
  BlockId *ids = malloc((maxDegree + 1) * sizeof(BlockId));
  Node node = (Node){
      .n = 0,
      .leaf = true,
      .data = NULL,
      .ids = NULL,
      .n_ids = 0};
  int sizeNodeStruct = sizeof(Node);
  int sizeData = sizeof(int) * maxDegree;
  int sizeIds = sizeof(BlockId) * (maxDegree + 1);
  int baseOffset = id;
  baseOffset *= (sizeNodeStruct + sizeData + sizeIds);
  // printf("read baseOffset: %i, %lu\n", baseOffset, id);
  // printf("re:\t %i\t %i\t %i\n", baseOffset, baseOffset + sizeNodeStruct, baseOffset + sizeNodeStruct + sizeData);

  pread(fildes, &node, sizeNodeStruct, baseOffset);
  node.data = data;
  node.ids = ids;
  pread(fildes, (int *)node.data, sizeData, baseOffset + sizeNodeStruct);
  pread(fildes, (BlockId *)node.ids, sizeIds, baseOffset + sizeNodeStruct + sizeData);
  return node;
}

void diskWrite(const Node node, BlockId id, int maxDegree)
{
  int sizeNodeStruct = sizeof(Node);
  int sizeData = sizeof(int) * maxDegree;
  int sizeIds = sizeof(BlockId) * (maxDegree + 1);
  int baseOffset = id;
  baseOffset *= (sizeNodeStruct + sizeData + sizeIds);
  int status;
  // printf("%i, %i, %i\n", sizeNodeStruct, baseOffset);
  status = pwrite(fildes, &node, sizeNodeStruct, baseOffset);
  status = pwrite(fildes, node.data, sizeData, baseOffset + sizeNodeStruct);
  status = pwrite(fildes, node.ids, sizeIds, baseOffset + sizeNodeStruct + sizeData);
}

static void
freeNode(Node node)
{
  free(node.data);
  free(node.ids);
}

void diskClose()
{
  fclose(fd);
}

void diskOpen()
{
  fd = fopen("index.b", "r+");
}