#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "btree.h"


int btreeTest()
{
  int minDegree = 10;
  Btree *bt = btreeCreate(minDegree);
  Node *node_struct = bt -> root;
  assert(node_struct -> minDegree == minDegree);
  assert(node_struct -> n == 0);  
  assert(true == node_struct -> leaf);
  return 0;
}

int tmpTest()
{
  int minDegree = 10;
  Btree *bt = btreeCreate(minDegree);
  Node *node_struct = bt -> root;
  int *keyList = node_struct -> keys;
  for (int i = 0; i < minDegree; i++)
    {
      printf("%i\n", *(keyList + i));
    }
  return 0;
}

/* simple btree containing only root node */
int btreeSearchSimpleTest()
{
  int minDegree = 10;
  Btree *bt = btreeCreate(minDegree);
  Node *node_struct = bt -> root;
  node_struct -> n = 10;
  int *keyList = node_struct -> keys;
  ResultSet *resultSet = malloc(sizeof(struct ResultSet));
  
  for (int i = 0; i < node_struct -> n; i++)
    {
      keyList[i] = i;
    }
  for (int i = 0; i < node_struct -> n; i++)
    {
        btreeSearch(node_struct, i, resultSet);
	assert(i == resultSet -> idx);
    }
  printf("node size %i'n\n", sizeof(struct Node));
  printf("node  %i'n\n", sizeof(bool));
}

int btreeSearchThreeChildrenTest()
{
  int minDegree = 10;
  Btree *bt = btreeCreate(minDegree);
  Node *node_struct = bt -> root;
  node_struct -> n = 10;
  int *keyList = node_struct -> keys;
  ResultSet *resultSet = malloc(sizeof(struct ResultSet));

  int factor = 5;
  for (int i = 0; i < node_struct -> minDegree; i * factor)
    {
      keyList[i] = i;
    }
/*   for (int i = 0; i < 3; i++) */
/*     { */
      
/*     } */
}


int main()
{
  btreeTest();
  /* TMPTEST(); */
  btreeSearchSimpleTest();
}
