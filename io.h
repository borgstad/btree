typedef struct DiskNode DiskNode;


/* TODO: fix this hacky thing */
struct DiskNode
{
  int n; // next, in the number of unallocated nodes
  int listSize;
  int unallocatedNodes[1000000]; // list of unallocated nodes
};


Node *diskRead(Node *node, int i);
void diskWrite(Node *node);

/* auxiliary functions */

int getDiskOffset(u_int64_t id);
static int hash(unsigned char *str);
