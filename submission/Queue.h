/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*/

#ifndef QUEUE_H
#define QUEUE_H

// A basic queue.
typedef struct Queue * Queue;

/*
* Creates a new empty Queue.
* Time complexity: O(1)
*/
Queue newQueue();

/*
* Adds a value to the end of the Queue
* Time complexity: O(1)
*/
int addQueue(Queue queue, void * value);

/*
* Removes the first value in the queue and returns the value.
* Time complexity: O(1)
*/
void * nextQueue(Queue queue);

/*
* Returns the first value in the queue.
* Time complexity: O(1)
*/
void * peakQueue(Queue queue);

/*
* Checks if the queue is empty, returns 1 if it isn't and 0 otherwise.
* Time complexity: O(1)
*/
int emptyQueue(Queue queue);

/*
* Returns the current size of the queue (how many values are stored in
* the queue).
*
* Time complexity: O(1)
*/
int sizeQueue(Queue queue);

// Provides a way to iterate over a Queue without destroying it.
typedef struct QueueIterator * QueueIterator;

/* 
* Creates a new iterater for a given Queue. The iterator is initalised to
* start at the beginning of the Queue. 

* Time complexity: O(1)
*/ 
QueueIterator newQueueIterator(Queue queue);

/* 
* Checks if the itterator is at the end of the Queue 
* (i.e. nextQueueIterator(QueueIterator iterator) would return NULL if called
* in the iterators current state). 
* 
* Time complexity: O(1)
*/  
int hasNextQueueIterator(QueueIterator iterator);

/* 
* Gets the next value from where the iterator is pointing in the Queue
* and moves the pointer forward one. 
*
* Time complexity: O(1)
*/
void * nextQueueIterator(QueueIterator iterator);

/*
* Gets the next value from where the iterator is pointing WITHOUT
* moving the pointer forward (i.e. No matter how many times this is
* called successively, the result will be the same).
*
* Time complexity: O(1)
*/
void * peakQueueIterator(QueueIterator iterator);

/*
* Moves the iterator pointer back to the start of the queue.
*
* Time complexity: O(1)
*/
void resetQueueIterator(QueueIterator iterator);

#endif