#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "linked_list.h"

LinkedList *getLinkedList(int size)
{
  LinkedList *l = initializeLinkedList((Id)0, 0);
  for (int i = 1; i < size; i++)
    addLinkedList(l, (Id)i, i);
  return l;
}

void testLinkedListInsert(int iterations)
{
  LinkedList *l = getLinkedList(iterations);
  for (int i = 0; i < iterations; i++)
  {
    assert((Id)i == l->id);
    assert(i == l->disk_offset);
    l = l->next;
  }
}

void testLinkedListDelete(int iterations)
{
  LinkedList *list = getLinkedList(iterations);
  int itemToDelete[] = {1, 3, 5};
  for (int i = 0; i < 3; i++)
  {
    deleteLinkedList(list, itemToDelete[i]);
  }
  while (list->next)
  {

    for (int i = 0; i < 3; i++)
    {
      assert(itemToDelete[i] != list->id);
    }
    list = list->next;
  }
}

void testLinkedListGet(int iterations)
{
  LinkedList *list = getLinkedList(iterations);
  int value;
  for (int i = 0; i < iterations; i++)
  {
    assert(OK == getItemLinkedList(list, (Id)i, &value));
  }
  for (int i = iterations; i < iterations + 10; i++)
  {
    assert(NOITEM == getItemLinkedList(list, (Id)i, value));
  }
}

int main()
{
  printf("--- Linked List Test\n");
  testLinkedListInsert(500);
  testLinkedListDelete(100);
  testLinkedListGet(100);
  testLinkedListGet(1);
  printf("--- Linked List Test complete\n\n");
}