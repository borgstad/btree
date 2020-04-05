#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/random.h>
#include "btree.h"
#include "io.h"


Btree
btreeCreate(int minDegree)
{
  
  Btree BtreeStruct;
  Node *rootT = allocateNode(minDegree, 0);
  BtreeStruct.root = rootT;
  return BtreeStruct;
}

Node *
allocateNode(int minDegree, int levelOrderNr)
{
  int *keyList;
  u_int64_t *ids;
  Node *node_struct;

  int typeSize = sizeof(int);
  keyList = malloc(minDegree * typeSize);
  ids = malloc(sizeof(u_int64_t) * minDegree);
  
  for (int i = 0; i < minDegree; i++)
    {
      keyList[i] = 0;
      ids[i] = getId();
    }
  
  node_struct = malloc(sizeof(struct Node));
  node_struct -> n = 0;
  node_struct -> leaf = true;
  node_struct -> keys = keyList;
  node_struct -> minDegree = minDegree;
  node_struct -> levelOrderNr = levelOrderNr;
  node_struct -> ids = ids;
  // TODO: should children be allocated on insertion instead of at node allocation time?
  node_struct -> children = malloc(sizeof(struct Node) * minDegree);
  return node_struct;
}

ResultSet
btreeSearch(const Node *node, int k)
{
  int i = 0;
  int *keys = node -> keys;
  ResultSet resultSet;
  
  while (i <= node -> n && k > keys[i])
    {
      i = i + 1;
    }
  if (i <= node -> n && k == keys[i])
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
      Node *childNode = diskRead(node -> children[i], i);
      btreeSearch(childNode, k);
    }
}

static u_int64_t
getId()
{
  u_int64_t res;
  getrandom(&res, sizeof(u_int64_t), GRND_RANDOM);
  return res;
}
