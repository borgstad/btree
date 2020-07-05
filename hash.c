#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

/* djb2 hash function */
int hash(unsigned char *str, int hashTableSize)
{
  unsigned long hashVal = 5381;
  int c;

  for (int i = 0; i < 8; i++)
  {
    c = *str++;
    hashVal = ((hashVal << 5) + hashVal) + c; /* hash * 33 + c */
  }
  return (int)(hashVal % hashTableSize) + 1;
}

HashTable
createHashTable(int tableSize)
{
  HashTable hashTable = {.linkedLists = malloc(sizeof(LinkedList *) * tableSize),
                         .tableSize = tableSize,
                         .curOffset = 1,
                         .hashedN = 0,
                         .hashedMax = tableSize * 20,
                         .unallocatedSize = tableSize / 10000,
                         .unallocated = malloc(sizeof(int) * tableSize / 10000),
                         .unallocatedN = 0};
  for (int i = 0; i < hashTable.tableSize; i++)
  {
    hashTable.linkedLists[i] = NULL;
  }
  return hashTable;
}

int hashPut(HashTable *hashTable, Id id)
{
  if (hashTable->hashedN >= hashTable->hashedMax)
  {
    return HASHFULL;
  }
  int hashTableIdx = hash((unsigned char *)&id, hashTable->tableSize);
  int curOffset = hashTable->curOffset;
  //// TODO: case when an element is deleted.
  // if (hashTable->unallocatedN > 0)
  // {
  //   curOffset = hashTable->unallocated[hashTable->unallocatedN];
  //   hashTable->unallocatedN--;
  // }
  LinkedList *newList;

  if (!hashTable->linkedLists[hashTableIdx])
  {
    newList = initializeLinkedList(id, curOffset);
    hashTable->linkedLists[hashTableIdx] = newList;
  }
  else
  {
    addLinkedList(hashTable->linkedLists[hashTableIdx], id, curOffset);
  }
  hashTable->hashedN++;
  hashTable->curOffset++;
  return HASHOK;
}

int hashGet(const HashTable *hashTable, Id id, int *value)
{
  unsigned char *idConv = (unsigned char *)&id;
  int tableSize = hashTable->tableSize;
  LinkedList *linkedList = hashTable->linkedLists[hash(idConv, tableSize)];
  int retVal = getItemLinkedList(linkedList, id, value);
  if (retVal == OK)
  {
    return HASHOK;
  }
  else
  {
    return HASHEMPTY;
  }

  /* return hashTable -> table[hash((unsigned char *) &id, hashTable -> tableSize)]; */
}

int hashDelete(HashTable *hashTable, Id id)
{
  int idx = hash((unsigned char *)&id, hashTable->tableSize);
  if (hashTable->linkedLists[idx] == HASHEMPTY)
  {
    return HASHOK;
  }
  if (hashTable->hashedN != 0)
  {
    hashTable->unallocated[hashTable->unallocatedN] = hashTable->linkedLists[idx];
    hashTable->unallocatedN++;
    hashTable->linkedLists[idx] = HASHEMPTY;
    hashTable->hashedN--;
  }
  return HASHOK;
}
