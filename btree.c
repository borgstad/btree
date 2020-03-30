#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "btree.h"



Btree *btreeCreate(int minDegree)
{
  
  Btree *BtreeStruct = malloc(sizeof(struct Btree));
  Node *rootT = allocateNode(minDegree);
  BtreeStruct -> root = rootT;
  return BtreeStruct;
}

Node *allocateNode(int minDegree)
{
  int *keyList;
  Node *node_struct;

  int typeSize = sizeof(int);
  keyList = malloc(minDegree * typeSize);
  // printf("%i\n", keyList);
  for (int i = 0; i < minDegree; i++)
    {
      keyList[i] = 0;
    }
  node_struct = malloc(sizeof(struct Node));
  node_struct -> n = 0;
  node_struct -> leaf = true;
  node_struct -> keys = keyList;
  node_struct -> minDegree = minDegree;
  // TODO: should children be allocated on insertion instead of at node allocation time?
  node_struct -> children = malloc(sizeof(struct Node) * minDegree);
  return node_struct;
}

int btreeSearch(Node *node, int k, ResultSet *resultSet)
{
  int i = 0;
  int *keys = node -> keys;

  
  while (i <= node -> n && k > keys[i])
    {
      i = i + 1;
    }
  if (i <= node -> n && k == keys[i])
    {
      resultSet -> node = node;
      resultSet -> idx = i;
      return 1; 
    }
  else if (node -> leaf)
    {
      return 0;
    }
  else
    {
      Node *childNode = diskRead(node -> children[i], i);
      btreeSearch(childNode, k, resultSet);
    }
  return 1;
}

Node *diskRead(Node *node, int idx)
{
  FILE *fd = fopen("index.b", "w");  
  int filedes = fileno(fd);
  int nodeStructSize = sizeof(struct Node);
  Node *childNode = malloc(sizeof(struct Node));
  pread(filedes, &childNode, nodeStructSize, 0);
  return node;
}
