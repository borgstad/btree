#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "include/hash.h"

/* djb2 hash function */
int hash(unsigned char *str, int hashTableSize)
{
  unsigned long hashVal = 5381;
  int c;

  for (int i = 0; i < 4; i++)
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
                         .hashedN = 0,
                         .hashedMax = tableSize * 20};
  for (int i = 0; i < hashTable.tableSize; i++)
  {
    hashTable.linkedLists[i] = NULL;
  }
  return hashTable;
}

int hashPut(HashTable *hashTable, BlockId id, void *data)
{
  if (hashTable->hashedN >= hashTable->hashedMax)
  {
    return HASHFULL;
  }
  int hashTableIdx = hash((unsigned char *)&id, hashTable->tableSize);
  LinkedList *newList;
  if (!hashTable->linkedLists[hashTableIdx])
  {
    newList = initializeLinkedList(id, data);
    hashTable->linkedLists[hashTableIdx] = newList;
  }
  else
  {
    addLinkedList(hashTable->linkedLists[hashTableIdx], id, data);
  }
  hashTable->hashedN++;
  return HASHOK;
}

int hashUpdate(HashTable *hashTable, BlockId id, void *data)
{
  updateItemLinkedList(hashGetLinkedList(hashTable, id), id, data);
}

LinkedList *hashGet(HashTable *hashTable, BlockId id)
{

  return getItemLinkedList(hashGetLinkedList(hashTable, id), id);
}

LinkedList *hashGetLinkedList(HashTable *hashTable, BlockId id)
{
  unsigned char *idConv = (unsigned char *)&id;
  int tableSize = hashTable->tableSize;
  return hashTable->linkedLists[hash(idConv, tableSize)];
}

int hashDelete(HashTable *hashTable, BlockId id)
{
  int idx = hash((unsigned char *)&id, hashTable->tableSize);
  deleteLinkedList(hashTable->linkedLists[idx], id);
  hashTable->hashedN--;
  return HASHOK;
}
