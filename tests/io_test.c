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


int
ioSimpleTest()
{
  int minDegree = 10;
  Btree root = initializeFullBtree(minDegree);
  Node *nodeRoot = root.root;

  for (int i = 0; i < minDegree; i++)
    {
      printf("ID: %ldx\n", nodeRoot -> ids[i]);
    }

  int i = 0;
  diskWrite(nodeRoot -> children[i], nodeRoot -> ids[i]);
  diskRead(nodeRoot, nodeRoot -> ids[i]);

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
  ioSimpleTest();
  /* ioAdvancedTest();   */
}
