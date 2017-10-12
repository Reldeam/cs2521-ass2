#include <stdlib.h>

#include "Queue.h"

struct Node {
	void * value;
	struct Node * next;
};

typedef struct Node * Node;

Node newNode(void * value)
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

int addQueue(Queue queue, void * value)
{
	if(!queue) return 0;
	
	if(!queue->head) {
		queue->head = newNode(value);
		queue->tail = queue->head;
	}
	else {
		queue->tail->next = newNode(value);
		queue->tail = queue->tail->next;
	}
	
	queue->size++;
	return 1;
}

void * nextQueue(Queue queue)
{
	if(!queue || !queue->head) return NULL;
		
	Node head = queue->head;
	void * value = head->value;
		
	queue->head = head->next;
	if(!queue->head) queue->tail = NULL;
		
	free(head);
	queue->size--;
	return value;
}


void * peakQueue(Queue queue)
{
	if(!queue || !queue->head) return NULL;
	return queue->head->value;
}

int emptyQueue(Queue queue)
{
	return queue->size ? 0 : 1;
}

int sizeQueue(Queue queue)
{
	return queue->size;
}

struct QueueIterator {
    Queue queue;
    Node pointer;
};

QueueIterator newQueueIterator(Queue queue)
{
    QueueIterator iterator = malloc(sizeof(struct QueueIterator));
    
    iterator->queue = queue;
    iterator->pointer = queue->head;
    
    return iterator;
}

int hasNextQueueIterator(QueueIterator iterator)
{
    if(!iterator || !iterator->pointer) return 0;
    return(iterator->pointer ? 1 : 0);
}

void * nextQueueIterator(QueueIterator iterator)
{
    if(!iterator || !iterator->pointer) return NULL;
    Node result = iterator->pointer;
    if(iterator->pointer) iterator->pointer = iterator->pointer->next;
    return result->value;
}

void * peakQueueIterator(QueueIterator iterator)
{
    if(!iterator || !iterator->pointer) return NULL;
    return iterator->pointer->value;
}

void resetQueueIterator(QueueIterator iterator)
{
    iterator->pointer = iterator->queue->head;
}

