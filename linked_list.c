#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

linkedListStatus addLinkedList(LinkedList *list, Id k, int n)
{
    LinkedList *newListItem = initializeLinkedList(k, n);
    while (list->next)
    {
        list = list->next;
    }
    list->next = newListItem;
    return OK;
}

LinkedList *initializeLinkedList(Id k, int n)
{
    LinkedList *listItem = malloc(sizeof(LinkedList));
    listItem->id = k;
    listItem->disk_offset = n;
    listItem->next = NULL;
    return listItem;
}

linkedListStatus deleteLinkedList(LinkedList *list, Id k)
{
    LinkedList *prevList = list;
    while (list->next)
    {
        if (k == list->id)
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

linkedListStatus getItemLinkedList(LinkedList *list, Id id, int *value)
{
    while (list)
    {
        if (id == list->id)
        {
            *value = list->disk_offset;
            return OK;
        }
        list = list->next;
    }
    return NOITEM;
}