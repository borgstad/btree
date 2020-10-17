#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/random.h>
#include "include/btree.h"
#include "include/storage.h"
#include "include/linked_list.h"
#include "include/hash.h"

static Block block;
static FILE *fd;
static int fildes;

/**
 * initializeStorage - initializes the storage module used by the io
 * 
 * The storage module keeps an open file description to a file, 'index.b',
 * containing the data written to disk. The data on disk in written to the blockid of
 * the node multiplied by an offset. The blockid is a number starting from 0
 * (the root node), and each time the btree allocates a new node, the blockid
 * counter is incremented by 1. 
 */
void initializeStorage()
{
  block = (Block){
      NULL,
      0};
  fd = fopen("index.b", "w+");
  fildes = fileno(fd);
}

/**
 * getNewBlockId - returns a blockid and increments the blocid count by 1
 */
BlockId getNewBlockId()
{
  BlockId res = block.curBlockId;
  block.curBlockId++;
  return res;
}

/**
 * diskRead - reads a node from disk, using the blockid as offset
 * 
 * @id: blockid of the node to read
 * @maxDegree: max number of keys for the nodes
 */
Node *diskRead(BlockId id, int maxDegree)
{
  int *data = malloc(maxDegree * sizeof(int));
  BlockId *ids = malloc((maxDegree + 1) * sizeof(BlockId));

  Node *node = malloc(sizeof(Node));
  node->n = 0;
  node->leaf = true;
  node->data = data;
  node->ids = ids;
  node->n_ids = 0;

  int sizeNodeStruct = sizeof(Node);
  int sizeData = sizeof(int) * maxDegree;
  int sizeIds = sizeof(BlockId) * (maxDegree + 1);
  int baseOffset = id;
  baseOffset *= (sizeNodeStruct + sizeData + sizeIds);

  // printf("read baseOffset: %i, %lu\n", baseOffset, id);
  // printf("re:\t %i\t %i\t %i\n", baseOffset, baseOffset + sizeNodeStruct, baseOffset + sizeNodeStruct + sizeData);

  pread(fildes, node, sizeNodeStruct, baseOffset);
  pread(fildes, (int *)node->data, sizeData, baseOffset + sizeNodeStruct);
  pread(fildes, (BlockId *)node->ids, sizeIds, baseOffset + sizeNodeStruct + sizeData);
  return node;
}

/**
 * diskWrite - writes a node to disk, using the blockid as offset
 * 
 * @node: pointer to the node that is to be written
 * @id: blockid of the node to read
 * @maxDegree: max number of keys for the nodes
 */
void diskWrite(Node *node, BlockId id, int maxDegree)
{
  int sizeNodeStruct = sizeof(Node);
  int sizeData = sizeof(int) * maxDegree;
  int sizeIds = sizeof(BlockId) * (maxDegree + 1);
  int baseOffset = id;
  baseOffset *= (sizeNodeStruct + sizeData + sizeIds);
  int status;
  // printf("%i, %i, %i\n", sizeNodeStruct, baseOffset);
  // printf("diskwrite %i\n", node->data[0]);
  // printf("diskwrite %i\n", node->data[1]);
  // printf("diskwrite %i\n", node->data[2]);
  status = pwrite(fildes, node, sizeNodeStruct, baseOffset);
  status = pwrite(fildes, node->data, sizeData, baseOffset + sizeNodeStruct);
  status = pwrite(fildes, node->ids, sizeIds, baseOffset + sizeNodeStruct + sizeData);
}

/**
 * diskFreeNode - frees a node
 * 
 * TODO: not used
 */
static void diskFreeNode(Node node)
{
  free(node.data);
  free(node.ids);
}

/**
 * diskClose - closes the open file description
 */
void diskClose()
{
  fclose(fd);
}

/**
 * diskOpen - opens 'index.b'
 */
void diskOpen()
{
  fd = fopen("index.b", "r+");
}