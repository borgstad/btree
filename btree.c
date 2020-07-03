#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/random.h>
#include "btree.h"
#include "io.h"

static Id getId();
static int MINDEGREE;
static int MAXDEGREE;
static int MAXKEYS;

Btree btreeInit(int minDegree)
{
  Btree BtreeStruct;
  MINDEGREE = minDegree;
  MAXDEGREE = 2 * minDegree - 1;
  MAXKEYS = 2 * minDegree;
  initialize(1000000, 10000000);
  Node rootT = btreeAllocateNode(0);
  rootT.leaf = true;
  diskWrite(rootT, 0, MAXDEGREE);
  BtreeStruct.root = rootT;
  BtreeStruct.id = 0;
  return BtreeStruct;
}

Node btreeAllocateNode(Id id)
{
  int *data;
  Id *ids;

  ids = malloc(MAXKEYS * sizeof(Id));
  for (int i = 0; i < MAXKEYS; i++)
  {
    ids[i] = getId();
  }

  Node node = (Node){
      .n = 0,
      .leaf = true,
      .data = malloc(MAXDEGREE * sizeof(int)),
      .ids = ids,
      .n_ids = 0};
  registerNode(id);
  // TODO: Write on creation or not?
  // diskWrite(node, id, MAXDEGREE);
  return node;
}

ResultSet
btreeSearch(const Node node, int k)
{
  int i = 0;
  ResultSet resultSet;

  while (i <= node.n && k > node.data[i])
  {
    i = i + 1;
  }
  if (i <= node.n && k == node.data[i])
  {
    resultSet.ok = true;
    resultSet.node = node;
    resultSet.idx = i;
    return resultSet;
  }
  else if (node.leaf)
  {
    resultSet.ok = false;
    return resultSet;
  }
  else
  {
    Node childNode = diskRead(node.ids[i], MAXDEGREE);
    return btreeSearch(childNode, k);
  }
}

static Id
getId()
{
  Id res;
  getrandom(&res, sizeof(Id), GRND_RANDOM);
  return res;
}

void btreeSplitChild(Node x, Id x_id, int index)
{
  int t = MINDEGREE;

  // new node going to id right of x
  Id z_id = getId();
  Node z = btreeAllocateNode(z_id);

  // node on the left of x, currently full
  Id y_id = x.ids[index];
  Node y = diskRead(x.ids[index], MAXDEGREE);

  z.leaf = y.leaf;
  z.n = t - 1;

  // populate right node with data from x
  for (int j = 0; j < t - 1; j++)
  {
    z.data[j] = y.data[j + t];
  }
  // if not leaf, also populate z with data
  if (!y.leaf)
  {
    for (int j = 0; j < t; j++)
    {
      z.ids[j] = y.ids[j + t];
    }
  }
  y.n = t - 1;
  // x.n = zero-indexing
  for (int j = x.n_ids; j >= index; j--)
  {
    x.ids[j + 1] = x.ids[j];
  }
  x.ids[index + 1] = z_id;
  for (int j = x.n - 1; j >= index; j--)
  {
    x.data[j + 1] = x.data[j];
  }
  x.data[index] = y.data[t - 1];
  x.n++;
  if (x.n_ids == 0)
  {
    x.n_ids++;
  }
  // printf("x_nids %i\n", x.n_ids);
  x.n_ids++;
  diskWrite(y, y_id, MAXDEGREE);
  diskWrite(z, z_id, MAXDEGREE);
  diskWrite(x, x_id, MAXDEGREE);
}

// insert a value into a btree with a nonfull root
void btreeInsertNonfull(Node x, Id x_id, int value)
{
  int i = x.n - 1;
  if (x.leaf)
  {
    while ((i >= 0) && (value < x.data[i]))
    {
      x.data[i + 1] = x.data[i];
      i--;
    }
    // printf("value %i, i %i\n", value, i);
    x.data[i + 1] = value;
    x.n++;
    diskWrite(x, x_id, MAXDEGREE);
  }
  else
  {
    while ((i >= 1) && (value < x.data[i]))
    {
      i--;
    }
    i++;
    Id x_child_id = x.ids[i];
    Node x_child = diskRead(x_child_id, MAXDEGREE);
    if (x_child.n == MAXDEGREE)
    {
      x = diskRead(x_id, MAXDEGREE);
      btreeSplitChild(x, x_id, i);
      if (value > x.data[i])
      {
        i++;
        x_child_id = x.ids[i];
      }
    }
    x_child = diskRead(x_child_id, MAXDEGREE);
    btreeInsertNonfull(x_child, x_child_id, value);
  }
}

Btree btreeInsert(Btree T, int value)
{
  Node r = T.root;
  if (r.n == MAXDEGREE)
  {
    Id newRootId = getId();
    Node s = btreeAllocateNode(newRootId);
    T.root = s;
    s.leaf = false;
    s.n = 0;
    s.ids[0] = T.id;
    T.id = newRootId;
    diskWrite(s, T.id, MAXDEGREE);
    // s = diskRead(T.id, MAXDEGREE);

    // printf("s_nids: %i\n", s.n_ids);
    btreeSplitChild(s, newRootId, 0);
    s = diskRead(newRootId, MAXDEGREE);
    // printf("s_nids: %i\n", s.n_ids);

    btreeInsertNonfull(s, newRootId, value);
    s = diskRead(newRootId, MAXDEGREE);
    // printf("s %i\n", s.n_ods);
    // s = diskRead(newRootId, MAXDEGREE);
    diskWrite(s, T.id, MAXDEGREE);
  }
  else
  {
    // printf("else: r %i\n", r.n);
    btreeInsertNonfull(r, T.id, value);
    r = diskRead(T.id, MAXDEGREE);
    T.root = r;
  }
  return T;
}