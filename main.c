#include <stdio.h>
#include "btree.h"
#include "io.h"

int main()
{
  int minDegree = 3;
  printf("lol\n");
  int maxDegree = minDegree * 2 - 1;
  Btree bt = btreeInit(minDegree);
  for (int i = 0; i < maxDegree + 14; i++)
  {
    bt = btreeInsert(bt, i);
    bt.root = diskRead(bt.id, maxDegree);
  }
}
