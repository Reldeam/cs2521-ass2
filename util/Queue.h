#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue * Queue;

Queue newQueue();

int addQueue(Queue queue, void * value);

void * nextQueue(Queue queue);
void * peakQueue(Queue queue);

int emptyQueue(Queue queue);
int sizeQueue(Queue queue);

typedef struct QueueIterator * QueueIterator;
QueueIterator newQueueIterator(Queue queue);
int hasNextQueueIterator(QueueIterator iterator);
void * nextQueueIterator(QueueIterator iterator);
void * peakQueueIterator(QueueIterator iterator);
void resetQueueIterator(QueueIterator iterator);

#endif