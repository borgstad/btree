#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"


int
hashPutTest()
{
  int hashTableSize = 1000000;
  HashTable hashTable = createHashTable(hashTableSize);

  assert(hashTableSize == hashTable.tableSize);
  assert(hashTableSize / 10000 == hashTable.unallocatedSize);
  assert(1 == hashTable.curOffset);
  assert(0 == hashTable.unallocatedN);
  /* Id id = getId(); */
  /* TODO: cannot test function getId since it is static, what to do? */
  Id id = 1128974;
  /* printf("%i\n", hash((unsigned char *) &id, hashTableSize)); */
  hashPut(&hashTable, id);

  /* printf("test\n"); */
  int curOffset = hashTable.table[hash((unsigned char *) &id, hashTableSize)];
  assert(2 == hashTable.curOffset);
  return 0;
}

int
hashGetTest()
{
  int hashTableSize = 10000000;
  HashTable hashTable = createHashTable(hashTableSize);
  srandom(0);

  Id *ids;
  int idsSize = hashTable.unallocatedSize;
  ids = malloc(sizeof(Id) * idsSize);
  Id test = 10000;
  hash((unsigned char *) &test, hashTableSize);

  int result;
  for (int i = 0; i < idsSize; i++)
    {
      assert(i == hashTable.hashedN);      
      ids[i] = (Id) random();
      result = hashPut(&hashTable, ids[i]);
      assert(HASHOK == result);
      assert(0 == hashTable.unallocatedN);
    }
  result = hashPut(&hashTable, ids[0]);
  assert(HASHFULL == result);
  
  for (int i = 0; i < idsSize; i++)
    {
      /* printf("%i\n", hashGet(&hashTable, ids[i])); */
      assert(i + 1 == hashGet(&hashTable, ids[i]));
    }
  free(ids);
}

int
hashDeleteTest()
{
  int hashTableSize = 10000000;
  HashTable hashTable = createHashTable(hashTableSize);
  srandom(0);

  Id *ids;
  int idsSize = hashTable.unallocatedSize;
  ids = malloc(sizeof(Id) * idsSize);
  Id test = 10000;
  hash((unsigned char *) &test, hashTableSize);

  int result;
  for (int i = 0; i < idsSize; i++)
    {
      ids[i] = (Id) random();
      result = hashPut(&hashTable, ids[i]);
    }
  
  for (int i = 0; i < 10; i++)
    {
      assert(i == hashTable.unallocatedN);
      hashDelete(&hashTable, ids[i]);
    }

  Id idPut; 
  for (int i = 10; i > 0; i--)
    {
      idPut = (Id) random();      
      assert(i == hashTable.unallocatedN);
      hashPut(&hashTable, ids[i]);
    }
}

int
main()
{
  hashPutTest();
  hashGetTest();
  hashDeleteTest();
}
