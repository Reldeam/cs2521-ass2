#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct PriorityQueue * PriorityQueue;

/**
* PriorityQueue places values into the queue based on the given
* compare method. Values the have higher priorities will be placed in-
* front of values with lower priorities. Matching priorities will be placed
* with older values being infront of newer values.
*/
PriorityQueue newPriorityQueue(int (* compare) (void * a, void * b));

/**
* Adds a value to the PriorityQueue. The same value can exist in the
* queque multiple times.
*/
void addToPriorityQueue(PriorityQueue queue, void * value);

/**
* Checks whether the queue has any values in it, returns 1 if there is
* at least one value and 0 otherwise.
*/
int isEmptyPriorityQueue(PriorityQueue queue);

/**
* Removes the value from the front of the queue and returns it.
*/
void * nextFromPriorityQueue(PriorityQueue queue);

/**
* Gets the value at the from of the queue without removing it and
* returns it.
*/
void * peakAtPriorityQueue(PriorityQueue queue);

/**
* Prints the queue, casting all values to (int) before printing.
*/
void printPriorityQueue(PriorityQueue queue);

/**
* Returns the number of values currently in the queue.
*/
int sizeOfPriorityQueue(PriorityQueue queue);

#endif