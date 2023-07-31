#ifndef _DLQ_C_
#define _DLQ_C_

#include <malloc.h>
#include "dlq.h"

#define TRUE 1
#define FALSE 0

typedef struct _dlnode_ {
    void *data;
    struct _dlnode_ *prev;
    struct _dlnode_ *next;
} DLNode;

typedef struct _dlqueue_ {
    struct _dlnode_ *first;
} DLQueue;

DLQueue *dlqCreate() {
    DLQueue *q;
    q = (DLQueue *)malloc(sizeof(DLQueue));
    if (q != NULL) {
        q->first = NULL;
    }
    return q;
}

int dlqEnqueue(DLQueue *q, void *item) {
    DLNode *newnode, *last = NULL;
    if (q != NULL) {
        newnode = (DLNode *)malloc(sizeof(DLNode));
        if (newnode != NULL) {
            newnode->data = item;
            newnode->next = NULL;
            if (q->first != NULL ) {
                last = q->first;
                while ( last->next != NULL ) {
                    last = last->next;
                }
            }
            newnode->prev = last;
            if (last != NULL) {
                last->next = newnode;
            } else {
                q->first = newnode;
            }
            return TRUE;
        }
    }
    return FALSE;
}

void *dlqDequeue(DLQueue *q) {
    void *data;
    DLNode *first, *nextnode;
    if (q != NULL) {
        if (q->first != NULL) {
            first = q->first;
            data = first->data;
            nextnode = first->next;
            if (nextnode != NULL) {
                nextnode->prev = NULL;
            }
            q->first = nextnode;
            free(first);
            return data;
        }
    }
    return NULL;
}

int dlqDestroy(DLQueue *q) {
    if (q != NULL) {
        if (q->first == NULL) {
            free(q);
            return TRUE;
        }
    }
    return FALSE;	
}

#endif
