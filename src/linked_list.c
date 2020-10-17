#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "include/linked_list.h"

/**
 * linkedListInit - initializes a linked list item
 * 
 * @blockId: blockid of the node
 * @data: pointer to data. Can be of type arbitrary
 */
LinkedList *linkedListInit(BlockId blockId, void *data)
{
    LinkedList *listItem = malloc(sizeof(LinkedList));
    listItem->blockId = blockId;
    listItem->data = data;
    listItem->next = NULL;
    return listItem;
}

/**
 * linkedListAdd - traverses the linked list, and adds the element to the end.
 * 
 * @list: list to add element to
 * @n: blockid
 * @data: pointer to data
 */
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

/**
 * linkedListDelete - deletes the first element from a linked list, matching the
 * blockid
 * 
 * @list: list to delete from
 * @blockId: blockid to match
 */
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

/**
 * linkedListUpdate - updates the pointer of the first linked list elemnt
 * matching a blockid
 * 
 * @list: linked list
 * @blockId: block id to match
 * @data: pointer to data to insert instead of current
 */
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

/**
 * linkedListGet - gets the first data pointer, matching a blockid
 * 
 * @list: linked list
 * @blockId: block id to match
 */
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