#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "btree.h"
#include "io.h"

int
testBtreeInit(int minDegree)
{
  Btree bt = btreeInit(minDegree);
  Node nodeRoot = bt.root;
  assert(0 == nodeRoot.n);
  assert(true == nodeRoot.leaf);
  assert(0 == nodeRoot.n_ids);
  return 0;
}


int 
testBtreeCreateNode(int minDegree)
{
  Btree bt = btreeInit(minDegree);
  Node childNode = btreeCreateNode(bt.root.ids[0]);
  assert(0 == childNode.n);
  assert(true == childNode.leaf);
  assert(0 == childNode.n_ids);
  return 0;
}


Btree initializeFullBtree(int minDegree)
{
  Btree bt = btreeInit(minDegree);
  Node nodeRoot = bt.root;

  nodeRoot.leaf = false;
  nodeRoot.n = minDegree;
  int buffer = 0;
  int maxDegree = minDegree * 2 - 1;

  nodeRoot.n_ids = minDegree * 2;
  Node iter_node;
  iter_node = diskRead(0, maxDegree);
//   for (int i = 0; i < nodeRoot.n_ids; i++)
//     {
//       iter_node = btreeCreateNode(nodeRoot.ids[i]);
//   //     // for (int j = 0; j < maxDegree; j++)
//   //     //   {
//   //     //     iter_node.data[j] = (nodeRoot.n) * i + j + buffer;
//   //     //   }
//   //     // buffer++;
//   //     // nodeRoot.data[i] = (nodeRoot.n) * (i + 1) + buffer;
//   //     // buffer += 1;
//     }
  return bt;
}


// /* Check struct fields of Btree and nodes are as expected */
// int
// btreeTest()
// {
//   int minDegree = 10;
//   Btree bt = initializeFullBtree(minDegree);
//   Node *node = bt.root;
//   assert(minDegree == node -> minDegree);
//   assert(false == node -> leaf);
  
//   assert(minDegree * 2 - 1 == node -> maxDegree);  
//   assert(node -> n == minDegree);  
//   /* printf("val: %i\n", (int) node -> ids[0]); */
//   int test = 0;
//   for (int i = 0; i < node -> n; i++)
//     {
//       for (int j = 0; j < node -> n + 1; j++)
// 	{
// 	  assert(test == node -> children[i] -> data[j]);
// 	  test++;
// 	}
//       assert(test == node -> data[i]);
//       test++;
//     }
//   return 0;
// }

// /* simple btree containing only root node */
// int
// btreeSearchSimpleTest()
// {
//   int minDegree = 10;
//   Btree bt = btreeCreate(minDegree);
//   Node *node = bt.root;
//   int *keyList = node -> data;
//   ResultSet resultSet;
//   node -> n = 10;
  
//   for (int i = 0; i < node -> n; i++)
//     {
//       keyList[i] = i;
//     }
//   for (int i = 0; i < node -> n; i++)
//     {
//         resultSet = btreeSearch(node, i);
// 	assert(i == resultSet.idx);
//     }
//   printf("node size %i'n\n", (int) sizeof(Node));
//   printf("node  %i'n\n", (int) sizeof(bool));
// }

// int
// btreeSearchThreeChildrenTest()
// {
//   int minDegree = 10;
//   Btree bt = initializeFullBtree(minDegree);
//   Node *node = bt.root;
  
// }



int
main()
{
    testBtreeInit(10);
    testBtreeCreateNode(10);
    // initializeFullBtree(10);
//   btreeTest();
  /* TMPTEST(); */
  /* btreeSearchSimpleTest(); */
  /* testHashing(); */
  /* ioSimpleTest(); */
  /* ioAdvancedTest();   */
}
