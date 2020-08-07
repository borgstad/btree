#include "btree.h"

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef enum linkedListStatus
{
    OK,
    NOITEM
} linkedListStatus;

typedef struct LinkedList
{
    BlockId blockId;         // disk offset
    void *data;              // satellite information
    struct LinkedList *next; // next item
} LinkedList;

#endif

linkedListStatus addLinkedList(LinkedList *list, BlockId n, void *data);
linkedListStatus deleteLinkedList(LinkedList *list, BlockId n);
void *getItemLinkedList(LinkedList *list, BlockId blockId);
LinkedList *initializeLinkedList(BlockId n, void *data);
linkedListStatus updateItemLinkedList(LinkedList *list, BlockId blockId, void *data);