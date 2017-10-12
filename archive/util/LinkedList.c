#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "LinkedList.h"

struct Node {
	void * value;
	struct Node * previous;
	struct Node * next;
};

typedef struct Node * Node;

static Node newNode(void * value)
{
	Node node = malloc(sizeof(struct Node));
	node->value = value;
	node->previous = NULL;
	node->next = NULL;
	return node;
}

static void deleteNode(Node node)
{
	node->value = NULL;
	node->next = NULL;
	node->previous = NULL;
	free(node);
}

struct LinkedList {
	Node head;
	Node tail;
	int size;
};

LinkedList newLinkedList()
{
	LinkedList list = malloc(sizeof(struct LinkedList));
	
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	
	return list;
}

void addToLinkedList(LinkedList list, void * value)
{
	addToEndOfLinkedList(list, value);
}

void addToFrontOfLinkedList(LinkedList list, void * value)
{
	assert(list);
	
	Node node = newNode(value);
	
	if(!list->head) {
		list->head = node;
		list->tail = node;
	}
	else {
		node->next = list->head;
		list->head->previous = node;
		list->head = node;
	}
	
	list->size++;
}

void addToEndOfLinkedList(LinkedList list, void * value)
{
	assert(list);
	
	Node node = newNode(value);
	
	if(!list->head) {
		list->head = node;
		list->tail = node;
	}
	else {
		node->previous = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
	
	list->size++;
}

int insertIntoLinkedList(LinkedList list, int pos, void * value)
{
	assert(list);
	
	if(pos < 0 || pos > list->size) return 0;
	
	Node node = newNode(value);
	
	if(list->size == 0) {
		list->head = node;
		list->tail = node;
		list->size++;
		return 1;
	}
	
	if(pos == list->size) {
		list->tail->next = node;
		node->previous = list->tail;
		list->tail = node;
		list->size++;
		return 1;
	}
	
	Node currentNode = list->head;
	
	int i;
	for(i = 0; i < pos; i++) currentNode = currentNode->next;
	
	Node previousNode = currentNode->previous;
	
	if(previousNode) {
		node->previous = previousNode;
		previousNode->next = node;
	}
	else {
		list->head = node;
	}
	
	node->next = currentNode;
	currentNode->previous = node;
	
	list->size++;
	return 1;
}

void * removeFirstFromLinkedList(LinkedList list)
{
	assert(list);
	
	if(isEmptyLinkedList(list)) return NULL;
	
	void * value = list->head->value;
	
	Node garbageNode = list->head;
	list->head = list->head->next;
	
	if(list->head) list->head->previous = NULL;
	else list->tail = NULL;
	
	deleteNode(garbageNode);
	
	list->size--;
	return value;
}

void * removeLastFromLinkedList(LinkedList list)
{
	assert(list);
	
	if(isEmptyLinkedList(list)) return NULL;
	
	void * value = list->tail->value;
	
	Node garbageNode = list->tail;
	list->tail = list->tail->previous;
	
	if(list->tail) list->tail->next = NULL;
	else list->head = NULL;
	
	deleteNode(garbageNode);
	
	list->size--;
	return value;
}

void * removeFromLinkedList(LinkedList list, void * value)
{
	assert(list);
	
	if(isEmptyLinkedList(list)) return NULL;
	
	Node garbageNode = list->head;
	
	while(garbageNode) {
		if(garbageNode->value == value) break;
		garbageNode = garbageNode->next;
	}
	
	if(!garbageNode) return NULL;
	
	if(garbageNode->next) {
		garbageNode->next->previous = garbageNode->previous;
	}
	if(garbageNode->previous) {
		garbageNode->previous->next = garbageNode->next;
	}
	
	if(list->head == garbageNode) {
		list->head = garbageNode->next;
	}
	if(list->tail == garbageNode) {
		list->tail = garbageNode->previous;
	}
	
	deleteNode(garbageNode);
	list->size--;
	
	return value;
}

void * spliceLinkedList(LinkedList list, int pos)
{
	assert(list);
	
	if(pos < 0 || pos >= list->size) return NULL;
	
	Node garbageNode = list->head;
	
	int i;
	for(i = 0; i < pos; i++) garbageNode = garbageNode->next;
	
	if(garbageNode->next) {
		garbageNode->next->previous = garbageNode->previous;
	}
	if(garbageNode->previous) {
		garbageNode->previous->next = garbageNode->next;
	}
	
	if(list->head == garbageNode) {
		list->head = garbageNode->next;
	}
	if(list->tail == garbageNode) {
		list->tail = garbageNode->previous;
	}
	
	void * value = garbageNode->value;
	
	deleteNode(garbageNode);
	list->size--;
	
	return value;
}

int isInLinkedList(LinkedList list, void * value)
{
	assert(list);
	
	Node currentNode = list->head;
	
	while(currentNode) {
		if(currentNode->value == value) break;
		currentNode = currentNode->next;
	}
	
	if(currentNode) return 1;
	return 0;
}

void * getFirstFromLinkedList(LinkedList list)
{
	assert(list);
	
	if(!list || !list->head) return NULL;
	return list->head->value;
}

void * getLastFromLinkedList(LinkedList list)
{
	assert(list);
	
	if(!list || !list->tail) return NULL;
	return list->tail->value;
}

void * getFromLinkedList(LinkedList list, int pos)
{
	assert(list);
	
	if(pos < 0 || pos >= list->size) return NULL;
	
	Node currentNode = list->head;
	
	int i;
	for(i = 0; i < pos; i ++) currentNode = currentNode->next;
	
	return currentNode->value;
}

int sizeOfLinkedList(LinkedList list)
{
	assert(list);
	return list->size;
}


int isEmptyLinkedList(LinkedList list)
{
	assert(list);
	
	if(list->size == 0) return 1;
	return 0;
}

void printLinkedList(LinkedList list)
{
	assert(list);
	
	printf("[");
	
	Node currentNode = list->head;
	
	while(currentNode) {
		printf("%d", (int) currentNode->value);
		if(currentNode->next) printf(", ");
		currentNode = currentNode->next;
	}
	
	printf("]\n");
}