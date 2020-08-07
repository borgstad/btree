#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "include/linked_list.h"

linkedListStatus linkedListAdd(LinkedList *list, BlockId n, void *data)
{
    LinkedList *newListItem = linkedListInit(n, data);

    if (!list)
        return OK;

    while (list->next)
    {
        list = list->next;
    }
    list->next = newListItem;
    return OK;
}

LinkedList *linkedListInit(BlockId blockId, void *data)
{
    LinkedList *listItem = malloc(sizeof(LinkedList));
    listItem->blockId = blockId;
    listItem->data = data;
    listItem->next = NULL;
    return listItem;
}

linkedListStatus linkedListDelete(LinkedList *list, BlockId blockId)
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

linkedListStatus linkedListUpdate(LinkedList *list, BlockId blockId, void *data)
{
    while (list)
    {
        if (blockId == list->blockId)
        {
            list->data = data;
            Node *d = list->data;
            return OK;
        }
        list = list->next;
    }
    return NOITEM;
}

void *linkedListGet(LinkedList *list, BlockId blockId)
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