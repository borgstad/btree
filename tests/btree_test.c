#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "btree.h"
#include "io.h"

Btree initializeFullBtree(int minDegree)
{
  Btree bt = btreeInit(minDegree);
  Node nodeRoot = bt.root;

  int buffer = 0;
  int maxDegree = minDegree * 2 - 1;
  nodeRoot.n = maxDegree;
  nodeRoot.leaf = false;
  nodeRoot.n_ids = minDegree * 2;

  Node iter_node;
  Id cur_id;
  for (int i = 0; i < maxDegree; i++)
  {
    cur_id = nodeRoot.ids[i];

    iter_node = btreeAllocateNode(cur_id);
    iter_node.n = maxDegree;
    iter_node.n_ids = 0;
    iter_node.leaf = true;
    for (int j = 0; j < maxDegree; j++)
    {
      iter_node.data[j] = (nodeRoot.n) * i + j + buffer;
    }
    diskWrite(iter_node, cur_id, maxDegree);
    nodeRoot.data[i] = (nodeRoot.n) * (i + 1) + buffer;
    buffer++;
  }

  diskWrite(nodeRoot, 0, maxDegree);

  nodeRoot = diskRead(0, maxDegree);
  int value = 0;
  for (int i = 0; i < maxDegree; i++)
  {
    cur_id = nodeRoot.ids[i];
    iter_node = diskRead(cur_id, maxDegree);
    for (int j = 0; j < maxDegree; j++)
    {
      assert(value == iter_node.data[j]);
      value++;
    }
    assert(value == nodeRoot.data[i]);
    value++;
  }
  bt.root = nodeRoot;
  return bt;
}

void testBtreeSearch(int minDegree)
{
  Btree bt = initializeFullBtree(minDegree);
  Node nodeRoot = bt.root;
  int maxDegree = minDegree * 2 - 1;
  ResultSet resultSet = btreeSearch(nodeRoot, 6);
  assert(resultSet.ok);
}

void testBtreeSplitChild(int minDegree)
{
  int maxDegree = minDegree * 2 - 1;
  Btree bt = initializeFullBtree(minDegree);
  Node nodeRoot = bt.root;
  nodeRoot.n_ids = 4;
  nodeRoot.n = 3;
  btreeSplitChild(nodeRoot, 0, 1);
  Node iter_node;
  nodeRoot = diskRead(0, maxDegree);
  btreeSplitChild(nodeRoot, 0, 3);
  nodeRoot = diskRead(0, maxDegree);
  // printf("\n");
  for (int i = 0; i < nodeRoot.n_ids; i++)
  {
    iter_node = diskRead(nodeRoot.ids[i], maxDegree);
    for (int j = 0; j < iter_node.n; j++)
    {
      // printf("%i\n", iter_node.data[j]);
    }
    if (i < nodeRoot.n)
    {
      printf("%i -\n", nodeRoot.data[i]);
    }
  }
}

int *randomList(int listLength)
{
  int *randList = malloc(sizeof(int) * listLength);
  for (int i = 0; i < listLength; i++)
  {
    randList[i] = rand();
  }
  return randList;
}

void testBtreeInsertNonFullRoot(int minDegree)
{
  int maxDegree = minDegree * 2 - 1;
  int *randList;
  Node nodeRoot;
  for (int i = 0; i < 1; i++)
  {
    randList = randomList(maxDegree);
    Node nodeRoot = btreeInit(minDegree).root;
    for (int j = 0; j < maxDegree; j++)
    {
      btreeInsertNonfull(nodeRoot, 0, randList[j]);
      nodeRoot = diskRead(0, maxDegree);
    }

    int tmp = 0;
    for (int j = 0; j < nodeRoot.n; j++)
    {
      assert(tmp <= nodeRoot.data[j]);
      tmp = nodeRoot.data[j];
    }
    printf("\n");
    free(randList);
  }
}

void testBtreeInsertNonFullChild(int minDegree)
{
  int maxDegree = minDegree * 2 - 1;
  int *randList;

  Node nodeRoot = initializeFullBtree(minDegree).root;
  nodeRoot.n--;
  diskWrite(nodeRoot, 0, maxDegree);
  nodeRoot = diskRead(0, maxDegree);

  btreeInsertNonfull(nodeRoot, 0, 2);
}

void inorderTraversal(Node node, int maxDegree)
{
  if (node.leaf)
  {
    for (int i = 0; i < node.n; i++)
    {
      printf("%i\n", node.data[i]);
    }
  }
  else
  {
    for (int i = 0; i < node.n; i++)
    {
      inorderTraversal(diskRead(node.ids[i], maxDegree), maxDegree);
      printf("%i\n", node.data[i]);
    }
    inorderTraversal(diskRead(node.ids[node.n], maxDegree), maxDegree);
  }
}

long long timeInMilliseconds(void)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

void testBtreeBigInsert(int minDegree)
{
  int nInsertions = 1000;
  int maxDegree = minDegree * 2 - 1;
  Btree bt = btreeInit(minDegree);

  long t = timeInMilliseconds();
  for (int i = 0; i < nInsertions; i++)
  {
    bt = btreeInsert(bt, i);
    bt.root = diskRead(bt.id, maxDegree);
  }
  assert(500 > timeInMilliseconds() - t);
  // printf("Time in millis: %lli\n", timeInMilliseconds() - t);
  // printf("Time in millis per item: %f\n", (float)(timeInMilliseconds() - t) / nInsertions);
}

int main()
{
  initializeFullBtree(3);
  testBtreeSearch(5);
  // testBtreeSplitChild(4);
  testBtreeInsertNonFullRoot(2);
  testBtreeInsertNonFullChild(3);
  testBtreeBigInsert(3);
}