#include "btree.h"
#include "linked_list.h"

#ifndef HASH_H
#define HASH_H

typedef enum hashReturn
{
  HASHOK = -1,
  HASHEMPTY = -2,
  HASHFULL = -3
} hashReturn;

typedef struct
{
  LinkedList **linkedLists;
  int tableSize;
  int hashedN;
  int hashedMax;
} HashTable;
#endif

HashTable createHashTable(int tableSize);
int hash(unsigned char *str, int hashTableSize);
int hashPut(HashTable *hashTable, BlockId id, void *data);
void *hashGet(const HashTable *hashTable, BlockId id);
int hashDelete(HashTable *hashTable, BlockId id);
