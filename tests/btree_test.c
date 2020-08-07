#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>
#include "btree.h"
#include "storage.h"
#include "io.h"

Btree initializeFullBtree(int minDegree)
{
  Btree bt = btreeInit(minDegree);
  Node *nodeRoot = bt.root;

  int buffer = 0;
  int maxDegree = minDegree * 2 - 1;
  nodeRoot->n = maxDegree;
  nodeRoot->leaf = false;
  nodeRoot->n_ids = minDegree * 2;

  Node *iterNode;
  BlockId cur_id;

  diskWrite(nodeRoot, 0, maxDegree);
  Node *testNode = diskRead(0, maxDegree);

  for (int i = 0; i < maxDegree; i++)
  {
    cur_id = nodeRoot->ids[i];
    iterNode = btreeAllocateNode(cur_id);
    iterNode->n = maxDegree;
    iterNode->n_ids = 0;
    iterNode->leaf = true;
    for (int j = 0; j < maxDegree; j++)
    {
      iterNode->data[j] = (nodeRoot->n) * i + j + buffer;
    }
    diskWrite(iterNode, cur_id, maxDegree);
    nodeRoot->data[i] = (nodeRoot->n) * (i + 1) + buffer;
    buffer++;
  }

  diskWrite(nodeRoot, 0, maxDegree);
  nodeRoot = diskRead(0, maxDegree);

  nodeRoot = diskRead(0, maxDegree);
  int value = 0;
  for (int i = 0; i < maxDegree; i++)
  {
    cur_id = nodeRoot->ids[i];
    iterNode = diskRead(cur_id, maxDegree);
    for (int j = 0; j < maxDegree; j++)
    {
      assert(value == iterNode->data[j]);
      value++;
    }
    assert(value == nodeRoot->data[i]);
    value++;
  }
  bt.root = nodeRoot;
  return bt;
}

void testBtreeSearch(int minDegree)
{
  Btree bt = initializeFullBtree(minDegree);
  Node *nodeRoot = bt.root;
  int maxDegree = minDegree * 2 - 1;
  ResultSet resultSet = btreeSearch(nodeRoot, 6);

  // assert(resultSet.ok);
}

void testBtreeSplitChild(int minDegree)
{
  int maxDegree = minDegree * 2 - 1;
  Btree bt = initializeFullBtree(minDegree);
  Node *nodeRoot = bt.root;
  nodeRoot->n_ids = 4;
  nodeRoot->n = 3;
  btreeSplitChild(nodeRoot, 0, 1);
  Node *iterNode;
  nodeRoot = diskRead(0, maxDegree);
  btreeSplitChild(nodeRoot, 0, 3);
  nodeRoot = diskRead(0, maxDegree);
  for (int i = 0; i < nodeRoot->n_ids; i++)
  {
    iterNode = diskRead(nodeRoot->ids[i], maxDegree);
    for (int j = 0; j < iterNode->n; j++)
    {
      // printf("%i\n", iterNode->data[j]);
    }
    if (i < nodeRoot->n)
    {
      // printf("%i -\n", nodeRoot->data[i]);
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
    Node *nodeRoot = btreeInit(minDegree).root;
    for (int j = 0; j < maxDegree; j++)
    {
      btreeInsertNonfull(nodeRoot, 0, randList[j]);
      nodeRoot = diskRead(0, maxDegree);
    }

    int tmp = 0;
    for (int j = 0; j < nodeRoot->n; j++)
    {
      assert(tmp <= nodeRoot->data[j]);
      tmp = nodeRoot->data[j];
    }
    free(randList);
  }
}

void testBtreeInsertNonFullChild(int minDegree)
{
  int maxDegree = minDegree * 2 - 1;
  int *randList;

  Node *nodeRoot = initializeFullBtree(minDegree).root;
  nodeRoot->n--;
  diskWrite(nodeRoot, 0, maxDegree);
  nodeRoot = diskRead(0, maxDegree);

  btreeInsertNonfull(nodeRoot, 0, 2);
}

void inorderTraversal(Node *node, int maxDegree, LinkedList *res)
{
  // printf("n_ids: %i, n %i\n", node->n_ids, node->n);
  if (node->leaf)
  {
    for (int i = 0; i < node->n; i++)
    {
      // printf("%i\n", node->data[i]);
      addLinkedList(res, node->data[i], node->data[i]);
    }
  }
  else
  {
    for (int i = 0; i < node->n; i++)
    {
      inorderTraversal(ioRead(node->ids[i], maxDegree), maxDegree, res);
      addLinkedList(res, node->data[i], node->data[i]);
    }
    inorderTraversal(ioRead(node->ids[node->n], maxDegree), maxDegree, res);
  }
}

long long timeInMilliseconds(void)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

int getOptimalNodeSize()
{
  int pageSize = getpagesize();
  int size = (pageSize - sizeof(Node)) / sizeof(BlockId);
  return size;
}

void testBtreeBigInsert(int minDegree, int nInsertions)
{
  int maxDegree = minDegree * 2 - 1;
  Btree bt = btreeInit(minDegree);
  long t = timeInMilliseconds();
  for (int i = 0; i < nInsertions; i++)
  {
    bt = btreeInsert(bt, i);
    bt.root = diskRead(bt.id, maxDegree);
  }
  long insTime = timeInMilliseconds() - t;
  printf("Time in millis for %i sorted insertions: %lli\n", nInsertions, insTime);
  printf("Time in millis for %i sorted insertions: %.6f\n", nInsertions, (float)nInsertions / (float)(insTime));
  diskClose();
}

void testBtreeBigInsertRandom(int minDegree, int nrRandomValues)
{
  int maxDegree = minDegree * 2 - 1;
  Btree bt = btreeInit(minDegree);
  int *randList = randomList(nrRandomValues);

  long t = timeInMilliseconds();
  for (int i = 0; i < nrRandomValues; i++)
  {
    bt = btreeInsert(bt, randList[i]);
    // bt = btreeInsert(bt, i);
  }
  long insTime = timeInMilliseconds() - t;

  LinkedList *res = initializeLinkedList(0, NULL);
  inorderTraversal(bt.root, maxDegree, res);
  int i = 1;

  while ((res = res->next))
  {
    int prev = res->data;
    // printf("%i, %i, %i \n", i, prev, res->data);
    assert(prev <= res->data);
    i++;
  }
}

int main()
{
  printf("--- Btree Test\n");
  initializeFullBtree(3);
  // testBtreeSearch(5);
  // testBtreeSplitChild(4);
  // testBtreeInsertNonFullRoot(2);
  // testBtreeInsertNonFullChild(3);
  int minDegree = 3; //(getOptimalNodeSize() + 1) / 2;
  int nrValues = 160;
  // testBtreeBigInsert(minDegree, nrValues);
  testBtreeBigInsertRandom(minDegree, nrValues);
  printf("--- Btree Test complete\n\n");
}