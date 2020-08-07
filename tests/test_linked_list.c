#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include "include/linked_list.h"

LinkedList *getLinkedList(int size)
{
  LinkedList *l = initializeLinkedList(0, 0);
  for (int i = 1; i < size; i++)
    addLinkedList(l, i, i);
  return l;
}

void testLinkedListInsert(int iterations)
{
  LinkedList *l = getLinkedList(iterations);
  for (int i = 0; i < iterations; i++)
  {
    assert(i == l->blockId);
    l = l->next;
  }
}

void testLinkedListDelete(int iterations)
{
  LinkedList *list = getLinkedList(iterations);
  int itemToDelete[] = {1, 3, 5};
  for (int i = 0; i < 3; i++)
  {
    linkedListStatus returnStatus = deleteLinkedList(list, itemToDelete[i]);
    assert(returnStatus == OK);
  }
  while (list->next)
  {
    for (int i = 0; i < 3; i++)
    {
      assert(itemToDelete[i] != list->blockId);
    }
    list = list->next;
  }
}

void testDeleteSingleItem()
{
  LinkedList *list = getLinkedList(1);
  linkedListStatus returnStatus = deleteLinkedList(list, 1);
  assert(returnStatus == NOITEM);
}

void testLinkedListGet(int iterations)
{
  LinkedList *list = getLinkedList(iterations);
  int value;
  for (int i = 0; i < iterations; i++)
  {
    // assert(OK == getItemLinkedList(list, (BlockId)i, &value));
  }
  for (int i = iterations; i < iterations + 10; i++)
  {
    // assert(NOITEM == getItemLinkedList(list, (BlockId)i, value));
  }
}

int main()
{
  printf("--- Linked List Test\n");
  testLinkedListInsert(500);
  testLinkedListDelete(10);
  testDeleteSingleItem();
  testLinkedListGet(100);
  testLinkedListGet(1);
  printf("--- Linked List Test complete\n\n");
}