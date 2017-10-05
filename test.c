#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "util/BST.h"
#include "util/HashMap.h"
#include "util/PriorityQueue.h"

struct Node {
	char * name;
	int value;
};

typedef struct Node * Node;

Node newNode(char * name, int value)
{
	Node node = malloc(sizeof(struct Node));
	node->name = name;
	node->value = value;
	return node;
}

int compare(void * a, void * b)
{
	Node A = (Node) a;
	Node B = (Node) b;
	
	return A->value - B->value;
}

int main()
{
	BST tree = newBST(compare);
	PriorityQueue queue = newPriorityQueue(compare);
	
	Node node;
	
	
	node = newNode("A", 1);
	addToPriorityQueue(queue, node);
	insertIntoBST(tree, node);
	
	node = newNode("B", 2);
	addToPriorityQueue(queue, node);
	insertIntoBST(tree, node);
	
	node = newNode("C", 3);
	addToPriorityQueue(queue, node);
	insertIntoBST(tree, node);
	
	node = newNode("D", 4);
	addToPriorityQueue(queue, node);
	insertIntoBST(tree, node);
	
	
	while(!isEmptyPriorityQueue(queue)) {
		node = (Node) nextFromPriorityQueue(queue);
		printf("%s %d\n", node->name, node->value);
	}
	
	
	
	while(sizeOfBST(tree)) {
		printf("Size %d\n", sizeOfBST(tree));
		node = (Node) getMaxValueFromBST(tree);
		removeFromBST(tree, node);
		printf("%s %d\n", node->name, node->value);
		printf("Size %d\n", sizeOfBST(tree));
	}
	
	
	return 1;
}