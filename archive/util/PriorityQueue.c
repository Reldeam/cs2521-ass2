#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "PriorityQueue.h"
#include "BST.h"

struct PriorityQueue {
	BST tree;
};

PriorityQueue newPriorityQueue(int (* compare) (void * a, void * b))
{
	PriorityQueue queue = malloc(sizeof(struct PriorityQueue));
	
	queue->tree = newBST(compare);
	
	return queue;
}

void addToPriorityQueue(PriorityQueue queue, void * value)
{
	insertIntoBST(queue->tree, value);
}

int isEmptyPriorityQueue(PriorityQueue queue)
{
	return(isEmptyBST(queue->tree));
}

void * nextFromPriorityQueue(PriorityQueue queue)
{
	assert(queue);
	
	if(isEmptyBST(queue->tree)) return NULL;
	void * value = getMaxValueFromBST(queue->tree);
	removeFromBST(queue->tree, value);
	return value;
}

void * peakAtPriorityQueue(PriorityQueue queue)
{
	assert(queue);
	
	if(isEmptyBST(queue->tree)) return NULL;
	return getMaxValueFromBST(queue->tree);
}

void printPriorityQueue(PriorityQueue queue)
{
	assert(queue);
	
	BST tree = getCopyOfBST(queue->tree);
	void * value;
	
	printf("[");
	
	while(!isEmptyBST(tree)) {
		value = getMaxValueFromBST(tree);
		printf("%d", (int) value);
		removeFromBST(tree, value);
		if(!isEmptyBST(tree)) printf(", ");
	}
	printf("]\n");
}

int sizeOfPriorityQueue(PriorityQueue queue)
{
	assert(queue);
	
	return sizeOfBST(queue->tree);
}