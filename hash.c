#include <stdlib.h>
#include <stdio.h>
#include "hash.h"



/* djb2 hash function */
int
hash(unsigned char *str, int hashTableSize)
{
  unsigned long hashVal = 5381;
  int c;

  for (int i = 0; i < 8; i ++)
    {
      c = *str++;
      hashVal = ((hashVal << 5) + hashVal) + c; /* hash * 33 + c */
      /* printf("%i\n", hashVal); */
    }
  
  return (int) (hashVal % hashTableSize) + 1;
}

HashTable
createHashTable(int tableSize)
{
  HashTable hashTable = {.table = malloc(sizeof(int) * tableSize),
			 .tableSize = tableSize,
			 .curOffset = 1,
			 .unallocatedSize = 100,
			 .unallocated = malloc(sizeof(int) * tableSize / 1000),
			 .unallocatedN = 0};
  for (int i = 0; i < hashTable.tableSize; i++)
    {
      hashTable.table[i] = -1;
    }
  return hashTable;
}

void
hashPut(HashTable *hashTable, Id id)
{
  int hashTableIdx = hash((unsigned char *) &id, hashTable -> tableSize); 
  hashTable -> table[hashTableIdx] = hashTable -> curOffset;
  hashTable -> curOffset++;
}

int
hashGet(const HashTable *hashTable, Id id)
{
  return hashTable -> table[hash((unsigned char *) &id, hashTable -> tableSize)];
}
