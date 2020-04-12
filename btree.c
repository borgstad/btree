#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/random.h>
#include "btree.h"
#include "io.h"


static Id getId();

Btree
btreeCreate(int minDegree)
{
  
  Btree BtreeStruct;
  Node *rootT = allocateNode(minDegree);
  BtreeStruct.root = rootT;
  return BtreeStruct;
}

Node *
allocateNode(int minDegree)
{
  int *data;
  Id *ids;
  Node *node_struct;

  int typeSize = sizeof(int);
  data = malloc(minDegree * typeSize);
  ids = malloc(sizeof(Id) * minDegree);
  
  for (int i = 0; i < minDegree; i++)
    {
      ids[i] = getId();
    }
  
  node_struct = malloc(sizeof(Node));
  node_struct -> n = 0;
  node_struct -> leaf = true;
  node_struct -> data = data;
  node_struct -> minDegree = minDegree;
  node_struct -> maxDegree = 2 * minDegree - 1;
  node_struct -> ids = ids;
  // TODO: should children be allocated on insertion instead of at node allocation time?
  node_struct -> children = malloc(sizeof(Node) * (2 * minDegree - 1));
  return node_struct;
}

ResultSet
btreeSearch(const Node *node, int k)
{
  int i = 0;
  int *data = node -> data;
  ResultSet resultSet;
  
  while (i <= node -> n && k > data[i])
    {
      i = i + 1;
    }
  if (i <= node -> n && k == data[i])
    {
      resultSet.ok = true;
      resultSet.node = *node;
      resultSet.idx = i;
      return resultSet; 
    }
  else if (node -> leaf)
    {
      resultSet.ok = false;
      return resultSet;
    }
  else
    {
      Node *childNode = diskRead(node, node -> ids[i]);
      btreeSearch(childNode, k);
    }
}

static Id
getId()
{
  Id res;
  getrandom(&res, sizeof(Id), GRND_RANDOM);
  return res;
}
