#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/random.h>
#include "btree.h"
#include "io.h"


/* TODO: fix hack. listSize should be set somewhere else and unallocatedNodes should be dynamically
allocated */
DiskNode diskNode = { .n = 0, .listSize = 1000000};

Node *diskRead(Node *nodeIn, int i)
{
  printf("before fopen\n");

  Node *node = malloc(sizeof(struct Node));
  FILE *fd = fopen("index.b", "r");
  int filedes = fileno(fd);
  int nodeStructSize = sizeof(struct Node);
  int keySize = sizeof(int *) * nodeIn -> n;
  int childrenSize = sizeof(struct Node *) * (nodeIn -> n + 1);
  int nodeOffset = nodeStructSize + keySize + childrenSize;  
  int baseOffset = nodeOffset * nodeIn -> levelOrderNr;
  
  int *keyList = malloc(node -> minDegree * sizeof(int));

  int keyOffset = baseOffset + nodeOffset;
  Node **children = malloc(sizeof(struct Node) * nodeIn -> minDegree);
  Node *child;
  int childOffset = keyOffset + keySize;
  
  int *key;
  for (int i = 0; i < nodeIn -> n; i++)
    {
      
      key = malloc(sizeof(int));
      pread(filedes, &key, sizeof(int), keyOffset + sizeof(int) * i);

      child = malloc(sizeof(struct Node));
      pread(filedes, &child, childrenSize, childOffset + nodeStructSize * i);
      children[i] = child;
    }

  int levelOrderNr = node -> levelOrderNr + i + 1;
  printf("slol: %s\n", baseOffset * nodeIn -> levelOrderNr);
  pread(filedes, &node, nodeStructSize, baseOffset * levelOrderNr);
  printf("after reading: %i %i %i\n", node -> minDegree, levelOrderNr, node -> n);
  
  printf("after close\n");

  node -> keys = keyList;
  node -> children = children;
  fclose(fd);
  
  return node;
}

void
diskWrite(Node *node)
{
  FILE *fd = fopen("index.b", "w+");
  int fildes = fileno(fd);
  int nodeStructSize = sizeof(struct Node);
  int keySize = sizeof(int *) * node -> n;
  int childrenSize = sizeof(struct Node *) * (node -> n + 1);
  int idSize = sizeof(int);
  int nodeOffset = nodeStructSize + keySize + childrenSize;
  int baseOffset = nodeOffset * node -> levelOrderNr;

  pwrite(fildes, &node, nodeStructSize, baseOffset);    
  pwrite(fildes, &(node -> keys), keySize, baseOffset + nodeStructSize);

  Node children[node -> n];
  
  diskNode.n += 1;
  for (int i = 0; i < node -> n; i++)
    {
      children[i] = *(node -> children[i]);
      free(node -> children[i]);
    }
  pwrite(fildes, &(node -> ids), idSize, baseOffset + nodeStructSize + keySize);  
  
  free(node -> keys);
  free(node -> children);
  free(node);

  /* fclose(fd);   */
}

int
getDiskOffset(Id id)
{  
  unsigned char *str;
  /* hash((unsighed char *) id) */
  return 2;
}

/* djb2 hash function */
static int hash
(unsigned char *str)
{
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return (int) (hash % 1000000) + 1;
}
