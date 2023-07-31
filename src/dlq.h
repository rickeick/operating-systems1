#ifndef _DLQ_H_
#define _DLQ_H_

typedef struct _dlqueue_ DLQueue;

typedef struct _dlnode_ DLNode;

DLQueue *dlqCreate();

int dlqEnqueue(DLQueue *q, void *item);

void *dlqDequeue(DLQueue *q);

int dlqDestroy(DLQueue *q);

#endif
