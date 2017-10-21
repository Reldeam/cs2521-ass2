/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*
* PriorityQueue is an extention of BST, providing next and peak methods
* as well as alias for other methods.
*/

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>

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