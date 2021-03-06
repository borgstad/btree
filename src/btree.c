#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/random.h>
#include "include/btree.h"
#include "include/io.h"

static int MINDEGREE;
static int MAXDEGREE;
static int MAXKEYS;

/**
 * btreeInit - initialize a btree
 * @minDegree: minimum number of keys in each node
 *
 * Entry function for the btree. initializes a btree struct, and an associated
 * node. 
 * 
 * TODO: do not take degree as input, use OS pagesize
 */
Btree btreeInit(int minDegree)
{
  MINDEGREE = minDegree;
  MAXDEGREE = 2 * minDegree - 1;
  MAXKEYS = 2 * minDegree;
  ioInitialize();

  BlockId initalId = getNewBlockId();
  Btree BtreeStruct;
  Node *rootT = btreeAllocateNode();
  rootT->leaf = true;
  ioWrite(rootT, initalId, MAXDEGREE);
  BtreeStruct.root = rootT;
  BtreeStruct.id = initalId;
  return BtreeStruct;
}

/**
 * btreeAllocateNode - allocates a node
 * 
 * A node is allocated together with the max number of pointers to children,
 * such that * that allocation of children per node only happens once, in this
 * function. Note that a node does not know its own blockid, this is only known
 * by its parent.
 */
Node *btreeAllocateNode()
{
  int *data;
  BlockId *ids = malloc(MAXKEYS * sizeof(BlockId));
  for (int i = 0; i < MAXKEYS; i++)
  {
    ids[i] = getNewBlockId();
  }
  Node *node = malloc(sizeof(Node));
  node->n = 0;
  node->leaf = true;
  node->data = malloc(MAXDEGREE * sizeof(int));
  node->ids = ids;
  node->n_ids = 0;
  return node;
}

/**
 * btreeSearch - searches for a key recursively
 * 
 * @node: the start node
 * @k: value to search for
 */
ResultSet btreeSearch(Node *node, int k)
{
  int i = 0;
  ResultSet resultSet;

  while (i < node->n && k > node->data[i])
  {
    i++;
  }
  if (i <= node->n && k == node->data[i])
  {
    resultSet.ok = true;
    resultSet.node = node;
    resultSet.idx = i;
    return resultSet;
  }
  else if (node->leaf)
  {
    resultSet.ok = false;
    return resultSet;
  }
  else
  {
    Node *childNode = ioRead(node->ids[i], MAXDEGREE);
    return btreeSearch(childNode, k);
  }
}

/**
 * btreeSplitChild - split child procedure
 * 
 * @x: the node to split
 * @x_id: the id of the node to split
 * @index: the index to split at
 * 
 * Method desribed by Cormen et. al.
 */
void btreeSplitChild(Node *x, BlockId x_id, int index)
{
  int t = MINDEGREE;

  // new node going to id right of x
  BlockId z_id = getNewBlockId();
  Node *z = btreeAllocateNode();

  // node on the left of x, currently full
  BlockId y_id = x->ids[index];
  Node *y = ioRead(x->ids[index], MAXDEGREE);

  z->leaf = y->leaf;
  z->n = t - 1;

  // populate right node with data from x
  for (int j = 0; j < t - 1; j++)
  {
    z->data[j] = y->data[j + t];
  }
  // if not leaf, also populate z with data
  if (!y->leaf)
  {
    for (int j = 0; j < t; j++)
    {
      z->ids[j] = y->ids[j + t];
    }
  }
  y->n = t - 1;
  // x->n = zero-indexing
  for (int j = x->n; j >= index; j--)
  {
    x->ids[j + 1] = x->ids[j];
  }
  x->ids[index + 1] = z_id;
  for (int j = x->n - 1; j >= index; j--)
  {
    x->data[j + 1] = x->data[j];
  }
  x->data[index] = y->data[t - 1];
  x->n++;
  if (x->n_ids == 0)
  {
    x->n_ids++;
  }
  x->n_ids++;
  ioWrite(y, y_id, MAXDEGREE);
  ioWrite(z, z_id, MAXDEGREE);
  ioWrite(x, x_id, MAXDEGREE);
}

/**
 * btreeInsertNonfull - insert a value into a btree with a nonfull root
 * 
 * @x: the node to insert into
 * @x_id: the id of the node to insert into
 * @value: the value to insert
 * 
 * Method desribed by Cormen et. al.
 */
void btreeInsertNonfull(Node *x, BlockId x_id, int value)
{
  int i = x->n - 1;
  if (x->leaf)
  {
    while ((i >= 0) && (value < x->data[i]))
    {
      x->data[i + 1] = x->data[i];
      i--;
    }
    x->data[i + 1] = value;
    x->n++;
    ioWrite(x, x_id, MAXDEGREE);
  }
  else
  {
    // printf("else %i\n", x->data[i]);
    while ((i >= 0) && (value < x->data[i]))
    {
      i--;
    }
    i++;
    BlockId x_child_id = x->ids[i];
    Node *x_child = ioRead(x_child_id, MAXDEGREE);
    if (x_child->n == MAXDEGREE)
    {
      x = ioRead(x_id, MAXDEGREE);
      btreeSplitChild(x, x_id, i);
      if (value > x->data[i])
      {
        i++;
        x_child_id = x->ids[i];
      }
    }
    x_child = ioRead(x_child_id, MAXDEGREE);
    btreeInsertNonfull(x_child, x_child_id, value);
  }
}

/**
 * btreeInsert - inserts into a btree
 * 
 * @T: btree to insert into
 * @value: value to insert into btree
 * 
 * Main function for insertion to a btree. Method desribed by Cormen et. al.
 */
Btree btreeInsert(Btree T, int value)
{
  Node *r = T.root;
  if (r->n == MAXDEGREE)
  {
    BlockId newRootId = getNewBlockId();
    Node *s = btreeAllocateNode();
    T.root = s;
    s->leaf = false;
    s->n = 0;
    s->ids[0] = T.id;
    T.id = newRootId;
    btreeSplitChild(s, newRootId, 0);
    btreeInsertNonfull(s, newRootId, value);
  }
  else
  {
    btreeInsertNonfull(r, T.id, value);
    T.root = r;
  }
  return T;
}

void btreeFlush(int maxDegree)
{
  ioFlush(maxDegree);
}