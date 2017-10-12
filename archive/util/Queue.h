#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue * Queue;

Queue newQueue();

/**
* Adds the given value to the rear of the queue.
*
* @param Queue queue The queue to be added to.
* @param void * value The value to be added.
*/
void addToQueue(Queue queue, void * value);

/**
* Adds the given value to the front of the queue.
*
* @param Queue queue The queue to be added to.
* @param void * value The value to be added.
*/
void addToFrontOfQueue(Queue queue, void * value);

/**
* Whether the given Queue is empty or not.
*
* @returns 1 if the queue has at least one value, 0 otherwise.
*/
int isEmptyQueue(Queue queue);

/**
* Removes the next value from the front of the queue and returns it.
*
* @param Queue queue The queue to remove the next item from.
*
* @returns void * The next value in the queue, or NULL if the queue is
* empty.
*/
void * nextFromQueue(Queue queue);

/**
* Looks at the first value in the queue and returns it without removing it
* from the queue.
*
* @param Queue queue The queue to peak at.
*
* @returns void * The first value in the queue, or NULL if the queue is
* empty.
*/
void * peakAtQueue(Queue queue);

/**
* The current size of the queue (number of stored values).
*
* @param Queue queue The queue to retrieve the size of.
*
* @param int The current number of values stored in the queue.
*/
int sizeOfQueue(Queue queue);

#endif
