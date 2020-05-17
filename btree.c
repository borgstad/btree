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
  BtreeStruct.root = rootT;
  return BtreeStruct;
}


// Btree
// btreeCreate(int minDegree, int id)
// {
//   Btree BtreeStruct;
//   Node *rootT = btreeCreateNode(minDegree);
//   return BtreeStruct;
// }

Node
btreeCreateNode(Id id)
{
  int *data;
  Id *ids;
  Node node_struct;

  
  data = malloc(MAXDEGREE * sizeof(int));
  ids = malloc(MAXDEGREE * sizeof(Id));
  // for (int i = 0; i < MAXDEGREE; i++)
  //   {
  //     data[i] = -1;
  //     ids[i] = getId();
  //   }
  
  node_struct = (Node) {
    .n = 0,
    .leaf = true,
    .data = data,
    .ids = ids,
    .n_ids = 0};
  // node_struct.children = malloc(sizeof(Node) * (2 * minDegree));
  registerNode(id);
  diskWrite(node_struct, id, MAXDEGREE);
  return node_struct;
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