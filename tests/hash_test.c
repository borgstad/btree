#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include "hash.h"
#include "btree.h"

Id *idList;

void randomList(int listLength)
{
  Id *randList = malloc(sizeof(Id) * listLength);
  Id res;
  for (int i = 0; i < listLength; i++)
  {
    getrandom(&randList[i], sizeof(Id), GRND_RANDOM);
  }
  idList = randList;
}

HashTable getHashTable(int nrElements)
{
  int hashTableSize = 1000000;
  HashTable hashTable = createHashTable(hashTableSize);

  assert(hashTableSize == hashTable.tableSize);
  assert(hashTableSize / 10000 == hashTable.unallocatedSize);
  assert(1 == hashTable.curOffset);
  assert(0 == hashTable.unallocatedN);
  randomList(nrElements);

  int disk_offset, retVal;
  for (int i = 0; i < nrElements; i++)
  {
    assert(i + 1 == hashTable.curOffset);
    assert(i == hashTable.hashedN);
    hashPut(&hashTable, idList[i]);
  }
  return hashTable;
}

int hashGetTest(int nrElements)
{
  HashTable hashTable = getHashTable(nrElements);
  int result;
  int disk_offset;
  int retVal;
  for (int i = 0; i < nrElements; i++)
  {
    retVal = hashGet(&hashTable, idList[i], &disk_offset);
    assert(HASHOK == retVal);
    assert(i + 1 == disk_offset);
  }
}

// int hashDeleteTest()
// {

//   int result;
//   /* add data to hashtable */
//   ;
//   for (int i = 0; i < idsSize; i++)
//   {
//     ids[i] = (Id)random();
//     result = hashPut(&hashTable, ids[i]);
//   }
//   /* delete data from hashtable, and check that the number of unallocated elements is */
//   /*   increments */
//   for (int i = 0; i < 10; i++)
//   {
//     assert(i == hashTable.unallocatedN);
//     hashDelete(&hashTable, ids[i]);
//   }
//   /* put elements back into hash table and check that the unallocatedN is decremented */
//   Id idPut;
//   int curOffset = hashTable.curOffset;
//   for (int i = 10; i > 0; i--)
//   {
//     idPut = (Id)random();
//     assert(i == hashTable.unallocatedN);
//     hashPut(&hashTable, ids[i]);
//     assert(curOffset == hashTable.curOffset);
//   }
// }

int main()
{
  printf("--- Hash Test\n");
  hashGetTest(10000);
  // hashDeleteTest();
  printf("--- Hash Test complete\n\n");
}
