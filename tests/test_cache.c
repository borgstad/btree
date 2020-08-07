#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "include/btree.h"
#include "include/storage.h"
#include "include/cache.h"

Node **randomNodes(int nrNodes, int minDegree)
{
  // these nodes have n value and data set
  int maxDegree = minDegree * 2 - 1;
  Node **nodes = malloc(sizeof(Node *) * nrNodes);
  Node *node;
  for (int i = 0; i < nrNodes; i++)
  {
    node = btreeAllocateNode(i);
    node->n = maxDegree;
    for (int j = 0; j < maxDegree; j++)
    {
      node->data[j] = j + (i * maxDegree);
    }
    nodes[i] = node;
  }
  return nodes;
}

int cacheInit(int nrNodes, int minDegree)
{
  int maxDegree = minDegree * 2 - 1;
  Node **nodes = randomNodes(10, minDegree);
  Cache *cache = initializeCache(100);
  for (int i = 0; i < nrNodes; i++)
  {
    addItemCache(cache, i, nodes[i]);
  }
  Node *node;
  for (int i = 0; i < nrNodes; i++)
  {
    node = getCacheItem(cache, i);
    assert(maxDegree == node->n);
  }
  for (int i = nrNodes; i < nrNodes * 2; i++)
  {
    node = getCacheItem(cache, i);
    assert(NULL == node);
  }
}

int main()
{
  printf("--- Cache Test\n");
  cacheInit(10, 3);
  printf("--- Cache Test complete\n\n");
}