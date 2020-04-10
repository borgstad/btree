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
			 .hashedN = 0,
			 .hashedMax = tableSize / 10000,
			 .unallocatedSize = tableSize / 10000,
			 .unallocated = malloc(sizeof(int) * tableSize / 10000),
			 .unallocatedN = 0};
  for (int i = 0; i < hashTable.tableSize; i++)
    {
      hashTable.table[i] = HASHEMPTY;
    }
  return hashTable;
}

int
hashPut(HashTable *hashTable, Id id)
{
  if (hashTable -> hashedN >= hashTable -> hashedMax)
    {
      return HASHFULL;
    }
  int hashTableIdx = hash((unsigned char *) &id, hashTable -> tableSize);
  int curOffset = hashTable -> curOffset;
  if (hashTable -> unallocatedN > 0)
    {
      curOffset = hashTable -> unallocated[hashTable -> unallocatedN];
      hashTable -> unallocatedN--;
    }
  hashTable -> table[hashTableIdx] = curOffset;
  hashTable -> curOffset++;
  hashTable -> hashedN++;
  return HASHOK;
}

int
hashGet(const HashTable *hashTable, Id id)
{
  return hashTable -> table[hash((unsigned char *) &id, hashTable -> tableSize)];
}

int
hashDelete(HashTable *hashTable, Id id)
{
  int idx = hash((unsigned char *) &id, hashTable -> tableSize);
  if (hashTable -> table[idx] == HASHEMPTY)
    {
      return HASHOK;
    }
  if (hashTable -> hashedN != 0)
    {
      hashTable -> unallocated[hashTable -> unallocatedN] = hashTable -> table[idx];
      hashTable -> unallocatedN++;
      hashTable -> table[idx] = HASHEMPTY;
      hashTable -> hashedN--;
    }
  return HASHOK;
}

