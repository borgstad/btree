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
  int n;     // number of keys currently stored in Node
  bool leaf; // is this a leaf?
  int *data; // array of size n
  Id *ids;   // random unique id assigned to each node, for hashing
  int n_ids;
} Node;

typedef struct ResultSet
{
  bool ok; // if false, the node is empty and the result is not legal
  Node node;
  int idx;
} ResultSet;

/* TODO: minDegree and maxDegree should be a member of btree and not the node  */
typedef struct
{
  Node root;
  Id id;
} Btree;
#endif

Btree btreeInit(int minDegree);
Node btreeAllocateNode(Id id);
ResultSet btreeSearch(const Node node, int k);
void btreeSplitChild(Node x, Id x_id, int index);
void btreeInsertNonfull(Node x, Id x_id, int value);
Btree btreeInsert(Btree T, int value);