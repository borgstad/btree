#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "btree.h"
#include "io.h"

Btree initializeFullBtree(int minDegree)
{
  Btree bt = btreeCreate(minDegree);
  Node *nodeRoot = bt.root;

  nodeRoot -> leaf = false;
  nodeRoot -> n = minDegree;
  int buffer = 0;

  for (int i = 0; i < nodeRoot -> minDegree; i++)
    {
      nodeRoot -> children[i] = btreeCreate(minDegree).root;
      nodeRoot -> children[i] -> n = minDegree;
      
      for (int j = 0; j < nodeRoot -> n + 1; j++)
	{
	  nodeRoot -> children[i] -> data[j] = (nodeRoot -> n) * i + j + buffer;
	}
      buffer++;
      nodeRoot -> data[i] = (nodeRoot -> n) * (i + 1) + buffer;
      buffer += 1;
    }
  return bt;
}


/* Check struct fields of Btree and nodes are as expected */
int
btreeTest()
{
  int minDegree = 10;
  Btree bt = initializeFullBtree(minDegree);
  Node *node = bt.root;
  assert(minDegree == node -> minDegree);
  assert(false == node -> leaf);
  
  assert(minDegree * 2 - 1 == node -> maxDegree);  
  assert(node -> n == minDegree);  
  /* printf("val: %i\n", (int) node -> ids[0]); */
  int test = 0;
  for (int i = 0; i < node -> n; i++)
    {
      for (int j = 0; j < node -> n + 1; j++)
	{
	  assert(test == node -> children[i] -> data[j]);
	  test++;
	}
      assert(test == node -> data[i]);
      test++;
    }
  return 0;
}

/* simple btree containing only root node */
int
btreeSearchSimpleTest()
{
  int minDegree = 10;
  Btree bt = btreeCreate(minDegree);
  Node *node = bt.root;
  int *keyList = node -> data;
  ResultSet resultSet;
  node -> n = 10;
  
  for (int i = 0; i < node -> n; i++)
    {
      keyList[i] = i;
    }
  for (int i = 0; i < node -> n; i++)
    {
        resultSet = btreeSearch(node, i);
	assert(i == resultSet.idx);
    }
  printf("node size %i'n\n", (int) sizeof(Node));
  printf("node  %i'n\n", (int) sizeof(bool));
}

int
btreeSearchThreeChildrenTest()
{
  int minDegree = 10;
  Btree bt = initializeFullBtree(minDegree);
  Node *node = bt.root;
  
}



int
main()
{
  btreeTest();
  /* TMPTEST(); */
  /* btreeSearchSimpleTest(); */
  /* testHashing(); */
  /* ioSimpleTest(); */
  /* ioAdvancedTest();   */
}
