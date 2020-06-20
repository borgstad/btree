#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "btree.h"
#include "io.h"

int testDiskReadWrite(int minDegree)
{
  Btree bt = btreeInit(minDegree);
  Node nodeRoot = bt.root;
  int maxDegree = minDegree * 2 - 1;
  for (int i = 0; i < maxDegree; i++)
  {
    nodeRoot.data[i] = i * 5000;
  }
  diskWrite(nodeRoot, 0, maxDegree);
  Node loadedNode = diskRead(0, maxDegree);

  assert(0 == loadedNode.n);
  assert(true == loadedNode.leaf);
  assert(0 == loadedNode.n_ids);
  for (int i = 0; i < maxDegree; i++)
  {
    assert(i * 5000 == nodeRoot.data[i]);
    // printf("%i, %i\n", nodeRoot.data[i], loadedNode.data[i]);
    // printf("%lu\t %lu\n", nodeRoot.ids[i], loadedNode.ids[i]);
    assert(nodeRoot.ids[i] == loadedNode.ids[i]);
    assert(nodeRoot.data[i] == loadedNode.data[i]);
  }
}

int testBtreeCreateNode(int minDegree)
{
  int maxDegree = minDegree * 2 - 1;
  Node node = btreeInit(minDegree).root;
  Node tmp_node;
  int nr_nodes = 3;

  for (int i = 0; i < nr_nodes; i++)
  {
    tmp_node = btreeAllocateNode(node.ids[i]);
    for (int j = 0; j < maxDegree; j++)
    {
      tmp_node.data[j] = i * 10 + 100;
    }
    diskWrite(tmp_node, node.ids[i], maxDegree);
  }
  assert(0 == node.n);
  assert(true == node.leaf);
  assert(0 == node.n_ids);
  for (int i = 0; i < nr_nodes; i++)
  {
    tmp_node = diskRead(node.ids[i], maxDegree);
    for (int j = 0; j < maxDegree; j++)
    {
      assert(i * 10 + 100 == tmp_node.data[i]);
      // printf("%i\n", tmp_node.data[i]);
    }
  }
}

Btree initializeFullBtree(int minDegree)
{
  Btree bt = btreeInit(minDegree);
  Node nodeRoot = bt.root;
  int maxDegree = minDegree * 2 - 1;
  int buffer = 0;
  // nodeRoot.n = maxDegree;

  Node iter_node;
  for (int i = 0; i < maxDegree; i++)
  {
    // printf("%i\n", nodeRoot.ids[i]);
    iter_node = btreeAllocateNode(nodeRoot.ids[i]);
    iter_node.n = maxDegree;
    for (int j = 0; j < maxDegree; j++)
    {
      iter_node.data[j] = (nodeRoot.n) * i + j + buffer;
    }
    diskWrite(iter_node, nodeRoot.ids[i], maxDegree);
    buffer++;
    nodeRoot.data[i] = (nodeRoot.n) * (i + 1) + buffer;
    buffer += 1;
  }

  iter_node = diskRead(nodeRoot.ids[0], maxDegree);
  for (int j = 0; j < maxDegree; j++)
  {
    // printf("%i\n", iter_node.data[j]);
  }

  // for (int i = 0; i < maxDegree; i++)
  // {
  //   // printf("%i\n", nodeRoot.ids[i]);
  //   iter_node = diskRead(nodeRoot.ids[i], maxDegree);
  //   for (int j = 0; j < maxDegree; i++)
  //     {
  //       printf("%i\n", iter_node.data[i]);
  //     }
  //   printf("%i\n", nodeRoot.data[i]);
  // }
  // return bt;
}

int main()
{
  testDiskReadWrite(3);
  testBtreeCreateNode(5);
  // initializeFullBtree(10);
}