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
    Id id;                   // Id of node
    int disk_offset;         // disk offset
    struct LinkedList *next; // next item
} LinkedList;

#endif

linkedListStatus addLinkedList(LinkedList *list, Id k, int n);
linkedListStatus deleteLinkedList(LinkedList *list, Id k);
LinkedList *initializeLinkedList(Id id, int n);