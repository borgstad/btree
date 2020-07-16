#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

linkedListStatus addLinkedList(LinkedList *list, BlockId n, void *data)
{
    LinkedList *newListItem = initializeLinkedList(n, data);
    while (list->next)
    {
        list = list->next;
    }
    list->next = newListItem;
    return OK;
}

LinkedList *initializeLinkedList(BlockId n, void *data)
{
    LinkedList *listItem = malloc(sizeof(LinkedList));
    listItem->blockId = n;
    listItem->data = data;
    listItem->next = NULL;
    return listItem;
}

linkedListStatus deleteLinkedList(LinkedList *list, BlockId n)
{
    LinkedList *prevList = list;
    while (list->next)
    {
        if (n == list->blockId)
        {
            prevList->next = list->next;
            free(list);
            return OK;
        }
        prevList = list;
        list = list->next;
    }
    return NOITEM;
}

void *getItemLinkedList(LinkedList *list, BlockId blockId)
{
    while (list)
    {
        if (blockId == list->blockId)
        {
            return list->data;
        }
        list = list->next;
    }
    return NULL;
}