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
  Node *node = bt.root;
  assert(node -> minDegree == minDegree);
  assert(minDegree * 2 - 1 == node -> maxDegree);  
  assert(node -> n == 0);  
  printf("val: %i\n", (int) node -> ids[0]);
  assert(true == node -> leaf);
  return 0;
}

int
tmpTest()
{
  int minDegree = 10;
  Btree bt = btreeCreate(minDegree);
  Node *node = bt.root;
  int *keyList = node -> data;
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
  Btree bt = btreeCreate(minDegree);
  Node *node = bt.root;
  node -> n = 10;
  int *keyList = node -> data;

  int factor = 5;
  for (int i = 0; i < node -> minDegree; i * factor)
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
  Node *nodeChild = btreeCreate(minDegree).root;
  int childIdx = 0;

  /* abusing the btree create function to create a child and assign it to root node child  */
  nodeRoot -> children[childIdx] = nodeChild;
  Id childId = nodeRoot -> ids[childIdx];
  
  
  nodeChild -> n = 3;
  for (int i = 0; i < nodeChild -> n; i++)
    {
      nodeChild -> data[i] = i * 2;
      nodeChild -> children[i] = malloc(sizeof(Node));
    }
  printf("before write t, lorder, n  %i %i\n\n", nodeChild ->minDegree, nodeChild -> n);

  diskWrite(nodeChild, childId);
  /* diskRead(nodeRoot, 0); */

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
  Node *nodeChild = root.root;

  
  /* for (int i = 0; i < node -> minDegree) */
  /*   { */
      
  /*   } */
  
  printf("before write t, lorder, n  %i %i\n\n", nodeChild ->minDegree, nodeChild -> n);
  Node *nodeRead = malloc(sizeof(Node));
  /* diskWrite(nodeChild); */
  /* diskRead(nodeRead, levelOrderNr); */

  assert(nodeChild -> n == nodeRead -> minDegree);
  assert(nodeChild -> n == nodeRead -> n);
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
