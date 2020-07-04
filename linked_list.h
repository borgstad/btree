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
    int data;                // satellite information
    struct LinkedList *next; // next item
} LinkedList;

#endif

linkedListStatus addLinkedList(LinkedList *list, Id k);
linkedListStatus deleteLinkedList(LinkedList *list, Id k);
LinkedList *initializeLinkedList(Id id);