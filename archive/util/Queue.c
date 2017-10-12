#include <stdlib.h>
#include <assert.h>

#include "Queue.h"

struct Node {
	void * value;
	struct Node * next;
};

typedef struct Node * Node;

static Node newNode(void * value)
{
	Node node = malloc(sizeof(struct Node));
	
	node->value = value;
	node->next = NULL;
	
	return node;
}

struct Queue {
	Node head;
	Node tail;
	int size;
};

Queue newQueue()
{
	Queue queue = malloc(sizeof(struct Queue));
	
	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;
	
	return queue;
}

void addToQueue(Queue queue, void * value)
{
	assert(queue);
	
	Node node = newNode(value);
	
	if(!queue->head) queue->head = node;
	else queue->tail->next = node;
	queue->tail = node;
	
	queue->size++;
}

void addToFrontOfQueue(Queue queue, void * value)
{
	assert(queue);
	
	Node node = newNode(value);
	
	if(!queue->head) queue->tail = node;
	
	node->next = queue->head;
	queue->head = node;
	
	queue->size++;
}

void * nextFromQueue(Queue queue)
{
	assert(queue);
	
	if(!queue->head) return NULL;
		
	Node node = queue->head;
	void * value = node->value;
		
	queue->head = queue->head->next;
	queue->size--;
		
	free(node);
		
	return value;
}

void * peakAtQueue(Queue queue)
{
	assert(queue);
	
	if(!queue->head) return NULL;
	return queue->head->value;
}

int isEmptyQueue(Queue queue)
{
	assert(queue);
	
	if(queue->size > 0) return 0;
	return 1;
}

int sizeOfQueue(Queue queue)
{
	assert(queue);
	
	return queue->size;
}