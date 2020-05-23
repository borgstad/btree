#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/random.h>
#include "btree.h"
#include "io.h"


static Id getId();
static int MINDEGREE;
static int MAXDEGREE;

Btree
btreeInit(int minDegree)
{
  Btree BtreeStruct;
  MINDEGREE = minDegree;
  MAXDEGREE = 2 * minDegree - 1;
  initialize(1000000, 10000000);
  Node rootT = btreeCreateNode(0);
  diskWrite(rootT, 0, MAXDEGREE);
  BtreeStruct.root = rootT;
  return BtreeStruct;
}

Node
btreeCreateNode(Id id)
{
  int *data;
  Id *ids;
  
  ids = malloc(MAXDEGREE * sizeof(Id));
  for (int i = 0; i < MAXDEGREE; i++)
    {
      ids[i] = getId();
    }
  
  Node node = (Node) {
    .n = 0,
    .leaf = true,
    .data = malloc(MAXDEGREE * sizeof(int)),
    .ids = ids,
    .n_ids = 0};
  registerNode(id);
  // TODO: Write on creation or not?
  // diskWrite(node, id, MAXDEGREE);
  return node;
}

ResultSet
btreeSearch(const Node node, int k)
{
  int i = 0;
  int *data = node.data;
  ResultSet resultSet;
  
  while (i <= node.n && k > data[i])
    {
      i = i + 1;
    }
  if (i <= node.n && k == data[i])
    {
      resultSet.ok = true;
      resultSet.node = node;
      resultSet.idx = i;
      return resultSet; 
    }
  else if (node.leaf)
    {
      resultSet.ok = false;
      return resultSet;
    }
  else
    {
      Node childNode = diskRead(node.ids[i], MAXDEGREE);
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