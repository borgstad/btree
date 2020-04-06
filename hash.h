#include "btree.h"

typedef struct HashTable HashTable;

enum hashReturn {
		 HASHOK = -1,
		 HASHEMPTY = -2,
		 HASHFULL = -3
};
	     

struct HashTable
{
  int *table;
  int tableSize;
  int hashedN;
  int hashedMax;
  int curOffset;
  int unallocatedSize;
  int *unallocated;
  int unallocatedN;
};

int hash(unsigned char *str, int hashTableSize);
HashTable createHashTable(int tableSize);
int hashPut(HashTable *hashTable, Id id);
int hashGet(const HashTable *hashTable, Id id);
int hashDelete(HashTable *hashTable, Id id);
