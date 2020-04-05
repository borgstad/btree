#include <stdbool.h>
#include <stdint.h>
 
typedef struct Node Node;
typedef struct Btree Btree;
typedef struct ResultSet ResultSet;
typedef uint64_t Id;

/* extern FILE *fd; */

struct Node
{
  int n; // number of keys currently stored in Node
  bool leaf; // is this a leaf?
  int *keys; // array of size n
  int minDegree;
  int levelOrderNr; // level order traversal number
  struct Node **children; // n + 1 child Nodes
  Id *ids; // random unique id assigned to each node, for hashing
};

struct ResultSet
{
  bool ok; // if false, the node is empty and the result is not legal
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

