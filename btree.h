#include <stdbool.h>

 
typedef struct Node Node;
typedef struct Btree Btree;
typedef struct ResultSet ResultSet;

extern FILE *fd;

struct Node
{
  int n; // number of keys currently stored in Node
  bool leaf; // is this a leaf?
  int *keys; // array of size n
  int minDegree;
  struct Node **children; // n + 1 child Nodes
};

struct Btree
{
  Node *root;
};

struct ResultSet
{
  Node *node;
  int idx;
};

Btree *btreeCreate(int minDegree);
Node *allocateNode(int minDegree);
int btreeSearch(Node *node, int k, ResultSet *resultSet);
Node *diskRead(Node *node, int idx);
