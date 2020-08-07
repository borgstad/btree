#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <inttypes.h>
#include "include/hash.h"
#include "include/btree.h"

BlockId *idList;

void randomList(int listLength)
{
  BlockId *randList = malloc(sizeof(BlockId) * listLength);
  BlockId res;
  for (int i = 0; i < listLength; i++)
  {
    getrandom(&randList[i], sizeof(BlockId), GRND_RANDOM);
  }
  idList = randList;
}

HashTable getHashTable(int hashTableSize, int nrElementsInsert)
{
  HashTable hashTable = createHashTable(hashTableSize);

  assert(hashTableSize == hashTable.tableSize);
  randomList(nrElementsInsert);

  int disk_offset, retVal;
  for (int i = 0; i < nrElementsInsert; i++)
  {
    // printf("idList %" PRIu32 "\n", idList[i]);
    hashPut(&hashTable, idList[i], idList[i]);
  }
  return hashTable;
}

int hashGetTest(int nrElements)
{
  HashTable hashTable = getHashTable(1000, nrElements);
  int result;
  int disk_offset;
  BlockId *retVal;
  for (int i = 0; i < nrElements; i++)
  {
    retVal = hashGet(&hashTable, idList[i]);
    assert(idList[i] == retVal);
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
  hashGetTest(1000);
  // hashDeleteTest();
  printf("--- Hash Test complete\n\n");
}
