#include "include/btree.h"

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

linkedListStatus linkedListAdd(LinkedList *list, BlockId n, void *data);
linkedListStatus linkedListDelete(LinkedList *list, BlockId n);
void *linkedListGet(LinkedList *list, BlockId blockId);
LinkedList *linkedListInit(BlockId n, void *data);
linkedListStatus linkedListUpdate(LinkedList *list, BlockId blockId, void *data);