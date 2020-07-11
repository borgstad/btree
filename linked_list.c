#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

linkedListStatus addLinkedList(LinkedList *list, BlockId n)
{
    LinkedList *newListItem = initializeLinkedList(n);
    while (list->next)
    {
        list = list->next;
    }
    list->next = newListItem;
    return OK;
}

LinkedList *initializeLinkedList(BlockId n)
{
    LinkedList *listItem = malloc(sizeof(LinkedList));
    listItem->blockId = n;
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

linkedListStatus getItemLinkedList(LinkedList *list, BlockId blockId, int *value)
{
    while (list)
    {
        if (blockId == list->blockId)
        {
            *value = list->blockId;
            return OK;
        }
        list = list->next;
    }
    return NOITEM;
}