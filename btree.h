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
  int levelOrderNr; // level order traversal number
  struct Node **children; // n + 1 child Nodes
  u_int64_t *ids;
};

struct ResultSet
{
  bool ok;
  Node node;
  int idx;
};

struct Btree
{
  Node *root;
};

Btree btreeCreate(int minDegree);
Node *allocateNode(int minDegree, int level);
ResultSet btreeSearch(const Node *node, int k);

static u_int64_t getId();
