#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "btree.h"
#include "io.h"


int
btreeTest()
{
  int minDegree = 10;
  Btree bt = btreeCreate(minDegree);
  Node *node_struct = bt.root;
  assert(node_struct -> minDegree == minDegree);
  assert(node_struct -> n == 0);  
  assert(node_struct -> levelOrderNr == 0);
  printf("val: %i\n", node_struct -> ids[0]);
  assert(true == node_struct -> leaf);
  return 0;
}

int
tmpTest()
{
  int minDegree = 10;
  Btree bt = btreeCreate(minDegree);
  Node *node_struct = bt.root;
  int *keyList = node_struct -> keys;
  for (int i = 0; i < minDegree; i++)
    {
      printf("%i\n", *(keyList + i));
    }
  return 0;
}

/* simple btree containing only root node */
int
btreeSearchSimpleTest()
{
  int minDegree = 10;
  Btree bt = btreeCreate(minDegree);
  Node *node_struct = bt.root;
  int *keyList = node_struct -> keys;
  ResultSet resultSet;
  node_struct -> n = 10;
  
  for (int i = 0; i < node_struct -> n; i++)
    {
      keyList[i] = i;
    }
  for (int i = 0; i < node_struct -> n; i++)
    {
        resultSet = btreeSearch(node_struct, i);
	assert(i == resultSet.idx);
    }
  printf("node size %i'n\n", sizeof(struct Node));
  printf("node  %i'n\n", sizeof(bool));
}

int
btreeSearchThreeChildrenTest()
{
  int minDegree = 10;
  Btree bt = btreeCreate(minDegree);
  Node *node_struct = bt.root;
  node_struct -> n = 10;
  int *keyList = node_struct -> keys;

  int factor = 5;
  for (int i = 0; i < node_struct -> minDegree; i * factor)
    {
      keyList[i] = i;
    }
}

int
ioSimpleTest()
{
  int minDegree = 10;
  Btree root = btreeCreate(minDegree);
  Node *nodeRoot = root.root;
  Btree firstChild = btreeCreate(minDegree);
  Node *nodeWrite = firstChild.root;
  Node *nodeTmp;

  nodeRoot -> children[0] = nodeWrite;
  
  nodeWrite -> n = 3;
  for (int i = 0; i < nodeWrite -> n; i++)
    {
      nodeTmp = malloc(sizeof(struct Node));
      nodeWrite -> keys[i] = i * 2;
      nodeWrite -> children[i] = nodeTmp;
    }
  nodeWrite -> levelOrderNr = nodeRoot -> levelOrderNr + 1;
  

  int levelOrderNr = nodeWrite -> levelOrderNr;
  
  printf("before write t, lorder, n  %i %i %i\n\n", nodeWrite ->minDegree, nodeWrite -> levelOrderNr, nodeWrite -> n);

  diskWrite(nodeRoot);
  diskWrite(nodeWrite);
  diskRead(nodeRoot, 0);

  /* assert(nodeWrite -> n == nodeRead -> minDegree); */
  /* assert(nodeWrite -> n == nodeRead -> n); */
  /* assert(nodeWrite -> levelOrderNr == nodeRead -> levelOrderNr); */
}

int
ioAdvancedTest()
{
  int minDegree = 3;
  Btree root = btreeCreate(minDegree);
  Btree firstChild = btreeCreate(minDegree);
  Node *nodeWrite = root.root;  
  int levelOrderNr = nodeWrite -> levelOrderNr;

  
  /* for (int i = 0; i < node -> minDegree) */
  /*   { */
      
  /*   } */
  
  printf("before write t, lorder, n  %i %i %i\n\n", nodeWrite ->minDegree, nodeWrite -> levelOrderNr, nodeWrite -> n);
  Node *nodeRead = malloc(sizeof(struct Node));
  diskWrite(nodeWrite);
  diskRead(nodeRead, levelOrderNr);

  assert(nodeWrite -> n == nodeRead -> minDegree);
  assert(nodeWrite -> n == nodeRead -> n);
  assert(nodeWrite -> levelOrderNr == nodeRead -> levelOrderNr);  
}

int
testHashing()
{
  u_int64_t test_val = 33;
  for (int i = 0; i < 1000000; i+= 1)
    {
      int test = getDiskOffset(test_val);
      /* printf("%i\n", test); */
      assert(test < 1000000);
      assert(test > 0);
    }
  return 0;
}

int
main()
{
  btreeTest();
  /* TMPTEST(); */
  btreeSearchSimpleTest();
  /* testHashing(); */
  /* ioSimpleTest(); */
  /* ioAdvancedTest();   */
}
