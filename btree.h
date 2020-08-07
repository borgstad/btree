#include <stdbool.h>
#include <stdint.h>

/* typedef struct Node Node; */
/* typedef struct Btree Btree; */
/* typedef struct ResultSet ResultSet; */
typedef uint32_t BlockId;

/* extern FILE *fd; */

#ifndef NODE_H
#define NODE_H
typedef struct Node
{
  int n; // number of keys currently stored in Node
  int n_ids;
  bool leaf;    // is this a leaf?
  int *data;    // array of size n
  BlockId *ids; // incremental id directly related to disk offset
} Node;

typedef struct ResultSet
{
  bool ok; // if false, the node is empty and the result is not legal
  Node *node;
  int idx;
} ResultSet;

/* TODO: minDegree and maxDegree should be a member of btree and not the node  */
typedef struct
{
  Node *root;
  BlockId id;
} Btree;
#endif

Btree btreeInit(int minDegree);
Node *btreeAllocateNode(BlockId id);
ResultSet btreeSearch(Node *node, int k);
void btreeSplitChild(Node *x, BlockId x_id, int index);
void btreeInsertNonfull(Node *x, BlockId x_id, int value);
Btree btreeInsert(Btree T, int value);
void btreeFlush(int maxDegree);