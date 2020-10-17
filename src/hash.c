#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "include/hash.h"

/**
 * createHashTable - creates a hash table
 * 
 * @tableSize: number of entrances in the table
 * 
 * Hashing-with-chaining is used to avoid collisions. The blockid is the value
 * which is hashed. Each entrance of the hashtable contains a linkedlist, which
 * a pointer to a node
 */
HashTable createHashTable(int tableSize)
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

/**
 * hash - djb2 hash function
 * 
 * @str: pointer to input of arbitrary size
 * @hashTableSize: size of the hash table, used for modulo operation
 */
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

/**
 * hashPut - puts an item into a hash table
 * 
 * @hashTable: hash table to insert into
 * @id: blockid to hash
 * @data: pointer to data to insert
 */
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
    newList = linkedListInit(id, data);
    hashTable->linkedLists[hashTableIdx] = newList;
  }
  else
  {
    linkedListAdd(hashTable->linkedLists[hashTableIdx], id, data);
  }
  hashTable->hashedN++;
  return HASHOK;
}

/**
 * hashUpdate - updates the data pointer of a hashtable with given blockid
 * 
 * @hashTable: hash table to update
 * @id: blockid to hash
 * @data: pointer to data to update
 */
int hashUpdate(HashTable *hashTable, BlockId id, void *data)
{
  linkedListUpdate(hashGetLinkedList(hashTable, id), id, data);
}

/**
 * hashGet - gets data pointed to by blockid
 * 
 * @hashTable: hash table to search
 * @id: id to hash
 */
LinkedList *hashGet(HashTable *hashTable, BlockId id)
{

  return linkedListGet(hashGetLinkedList(hashTable, id), id);
}

/**
 * hashGetLinkedList - gets a linked list from a hash table, with id hashed
 * 
 * @hashTable: hash table to search
 * @id: id to hash
 */
LinkedList *hashGetLinkedList(HashTable *hashTable, BlockId id)
{
  unsigned char *idConv = (unsigned char *)&id;
  int tableSize = hashTable->tableSize;
  return hashTable->linkedLists[hash(idConv, tableSize)];
}

/**
 * hashDelete - delete an element from a hash table
 * 
 * @hashTable: hash table to delete from
 * @id: id to hash
 */
int hashDelete(HashTable *hashTable, BlockId id)
{
  int idx = hash((unsigned char *)&id, hashTable->tableSize);
  linkedListDelete(hashTable->linkedLists[idx], id);
  hashTable->hashedN--;
  return HASHOK;
}
