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
    struct LinkedList *next; // next item
} LinkedList;

#endif

linkedListStatus addLinkedList(LinkedList *list, BlockId n);
linkedListStatus deleteLinkedList(LinkedList *list, BlockId n);
linkedListStatus getItemLinkedList(LinkedList *list, BlockId blockId, int *value);
LinkedList *initializeLinkedList(BlockId n);