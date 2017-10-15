#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "BST.h"

typedef BST PriorityQueue;

PriorityQueue newPriorityQueue(int (*compare)(void * a, void * b));

int addPriorityQueue(PriorityQueue queue, void * value);

void * nextPriorityQueue(PriorityQueue queue);
void * peakPriorityQueue(PriorityQueue queue);

int existsPriorityQueue(PriorityQueue queue, void * value);

int emptyPriorityQueue(PriorityQueue queue);
int sizePriorityQueue(PriorityQueue queue);

void freePriorityQueue(PriorityQueue queue);

#endif