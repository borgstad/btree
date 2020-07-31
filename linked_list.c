#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "linked_list.h"

linkedListStatus addLinkedList(LinkedList *list, BlockId n, void *data)
{
    LinkedList *newListItem = initializeLinkedList(n, data);

    if (!list)
        return OK;

    while (list->next)
    {
        list = list->next;
    }
    list->next = newListItem;
    return OK;
}

LinkedList *initializeLinkedList(BlockId blockId, void *data)
{
    LinkedList *listItem = malloc(sizeof(LinkedList));
    listItem->blockId = blockId;
    listItem->data = data;
    listItem->next = NULL;
    return listItem;
}

linkedListStatus deleteLinkedList(LinkedList *list, BlockId blockId)
{
    if (!list)
        return NOITEM;

    LinkedList *prevList = list;
    LinkedList *curList = list->next;
    // LinkedList *nextList = list->next;
    int i = 0;
    while (curList)
    {
        if (blockId == curList->blockId)
        {
            prevList->next = curList->next;
            free(curList);
            return OK;
        }
        prevList = curList;
        curList = curList->next;
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