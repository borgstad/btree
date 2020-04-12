#include <stdbool.h>
#include <stdint.h>
 
/* typedef struct Node Node; */
/* typedef struct Btree Btree; */
/* typedef struct ResultSet ResultSet; */
typedef uint64_t Id;

/* extern FILE *fd; */

#ifndef NODE_H
#define NODE_H
typedef struct Node
{
  int n; // number of keys currently stored in Node
  bool leaf; // is this a leaf?
  int *data; // array of size n 
  int minDegree;
  int maxDegree;
  struct Node **children; // n + 1 child Nodes
  Id *ids; // random unique id assigned to each node, for hashing
} Node;

typedef struct 
{
  bool ok; // if false, the node is empty and the result is not legal
  Node node;
  int idx;
} ResultSet;

typedef struct 
{
  Node *root;
} Btree;
#endif

Btree btreeCreate(int minDegree);
Node *allocateNode(int minDegree);
ResultSet btreeSearch(const Node *node, int k);
