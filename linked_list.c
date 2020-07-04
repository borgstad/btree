#include <stdlib.h>
#include "linked_list.h"

linkedListStatus addLinkedList(LinkedList *list, Id k)
{
    LinkedList *newListItem = initializeLinkedList(k);
    while (list->next)
    {
        list = list->next;
    }
    list->next = newListItem;
    return OK;
}

LinkedList *initializeLinkedList(Id k)
{
    LinkedList *listItem = malloc(sizeof(LinkedList));
    listItem->data = k;
    listItem->next = NULL;
    return listItem;
}

linkedListStatus deleteLinkedList(LinkedList *list, Id k)
{
    LinkedList *prevList = list;
    while (list->next)
    {
        if (k == list->data)
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