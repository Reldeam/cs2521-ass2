/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*/

#include <stdlib.h>
#include "PriorityQueue.h"
#include "BST.h"

PriorityQueue newPriorityQueue(int (*compare)(void * a, void * b))
{
	PriorityQueue queue = newBST(compare);
	return queue;
}

int addPriorityQueue(PriorityQueue queue, void * value)
{
	return addBST(queue, value);
}

void * nextPriorityQueue(PriorityQueue queue)
{
	return removeBST(queue, maxBST(queue));
}

void * peakPriorityQueue(PriorityQueue queue)
{
	return maxBST(queue);
}

int existsPriorityQueue(PriorityQueue queue, void * value)
{
	return existsBST(queue, value);
}

int emptyPriorityQueue(PriorityQueue queue)
{
	return emptyBST(queue);
}

int sizePriorityQueue(PriorityQueue queue)
{
	return sizeBST(queue);
}

void freePriorityQueue(PriorityQueue queue)
{
	freeBST(queue);
}