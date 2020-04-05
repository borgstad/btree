#include "btree.h"

typedef struct HashTable HashTable;

struct HashTable
{
  int *table;
  int tableSize;
  int curOffset;
  int unallocatedSize;
  int *unallocated;
  int unallocatedN;
};

int hash(unsigned char *str, int hashTableSize);
HashTable createHashTable(int tableSize);
void hashPut(HashTable *hashTable, Id id);
int hashGet(const HashTable *hashTable, Id id);
