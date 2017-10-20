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
#include <stdio.h>

#include "BST.h"
#include "Queue.h"

struct Node
{
	void * value;
	int depth;
	struct Node * parent;
	struct Node * left;
	struct Node * right;
};

typedef struct Node * Node;

static Node newNode(void * value)
{
	Node node = malloc(sizeof(struct Node));
	
	node->value = value;
	node->depth = 0;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	
	return node;
}

struct BST
{
	Node root;
	int size;
	int (*compare)(void * a, void *b);
};

static Node addValue(BST tree, Node node, void * value);
static Node removeValue(BST tree, Node node, void * value);
static void * getValue(BST tree, Node node, void * value);
static int matchValue(BST tree, Node node, void * value);
static Node rotateRight(Node node);
static Node rotateLeft(Node node);
static Node minNode(Node node);
static Node maxNode(Node node);
static void updateDepth(Node node);
static void printNode(Node node, int indent);
static Queue getQueueNode(Queue queue, Node node);
static void freeNode(Node node);

BST newBST(int (*compare)(void * a, void * b))
{
	BST tree = malloc(sizeof(struct BST));
	
	tree->root = NULL;
	tree->size = 0;
	tree->compare = compare;
	
	return tree;
}

int addBST(BST tree, void * value)
{
	if(!tree) return 0;
	
	if(!tree->root) tree->root = newNode(value);
	else tree->root = addValue(tree, tree->root, value);
	
	tree->size++;
	
	return 1;
}

void * removeBST(BST tree, void * value)
{
	if(!tree) return NULL;
	
	void * result = removeValue(tree, tree->root, value);
	
	if(result) tree->size--;
	
	return result;
}

void * getBST(BST tree, void * value)
{
	if(!tree) return NULL;
	return getValue(tree, tree->root, value);
}

int existsBST(BST tree, void * value)
{
	if(!tree) return 0;
	return matchValue(tree, tree->root, value);
}

void printBST(BST tree)
{
	if(!tree || !tree->root) return;
	
	printNode(tree->root, 0);
}

int sizeBST(BST tree)
{
	return tree->size;
}

int emptyBST(BST tree)
{
	return tree->size ? 0 : 1;
}

void * rootBST(BST tree)
{
    if(!tree || !tree->root) return NULL;
    return tree->root->value;
}

void * maxBST(BST tree)
{
	if(!tree) return NULL;
	Node node = maxNode(tree->root);
	if(!node) return NULL;
	return node->value;
}

void * minBST(BST tree)
{
	if(!tree) return NULL;
	Node node = minNode(tree->root);
	if(!node) return NULL;
	return node->value;
}

static void printNode(Node node, int indent)
{
	if(!node) return;
	
	int i;
	printf("%d (%d)\n", (int) node->value, node->depth);
	
	if(node->left) {
		for(i = 0; i < indent; i++) printf(" ");
		printf("L: ");
		printNode(node->left, indent + 3);
	}
	
	if(node->right) {
		for(i = 0; i < indent; i++) printf(" ");
		printf("R: ");
		printNode(node->right, indent + 3);
	}
}

static Node addValue(BST tree, Node node, void * value)
{
	if(tree->compare(value, node->value) > 0) {
		if(node->right) addValue(tree, node->right, value);
		else {
			node->right = newNode(value);
			node->right->parent = node;
			
			if(!node->left) {
				node->depth++;
				updateDepth(node->parent);
			}
		}
	}
	else {
		if(node->left) addValue(tree, node->left, value);
		else {
			node->left = newNode(value);
			node->left->parent = node;
			
			if(!node->right) {
				node->depth++;
				updateDepth(node->parent);
			}
		}
	}
	
	int leftDepth = 0;
	int rightDepth = 0;
	if(node->left) leftDepth = 1 + node->left->depth;
	if(node->right) rightDepth = 1 + node->right->depth;
	
	if(leftDepth - rightDepth > 1) node = rotateRight(node);
	else if(rightDepth - leftDepth > 1) node = rotateLeft(node);
	
	return node;
}

static Node removeValue(BST tree, Node node, void * value)
{
	if(!tree || !node) return NULL;
	
	void * result;
	
	if(tree->compare(value, node->value) > 0) {
		result = removeValue(tree, node->right, value);
	}
	else if(tree->compare(value, node->value) < 0) {
		result = removeValue(tree, node->left, value);
	}
	else {
		result = node->value;
		if(node->left && node->right) {
			Node replacement = minNode(node->right);
			node->value = replacement->value;
			removeValue(tree, node->right, replacement->value);
		}
		else if(node->left) {
			if(node->parent) {
				if(node->parent->left == node)
					node->parent->left = node->left;
				else
					node->parent->right = node->left;
				node->left->parent = node->parent;
				updateDepth(node->parent);
			}
			else {
				tree->root = node->left;
				node->left->parent = NULL;
			}
			
			free(node);
			return result;
		}
		else if(node->right) {
			if(node->parent) {
				if(node->parent->left == node)
					node->parent->left = node->right;
				else
					node->parent->right = node->right;
				node->right->parent = node->parent;
				updateDepth(node->parent);
			}
			else {
				tree->root = node->right;
				node->right->parent = NULL;
			}
			
			free(node);
			return result;
		}
		else {
			if(node->parent) {
				if(node->parent->left == node) 
					node->parent->left = NULL;
				else 
					node->parent->right = NULL;
				updateDepth(node->parent);
			}
			else {
				tree->root = NULL;
			}
			
			free(node);
			return result;
		}
	}
	
	int leftDepth = 0;
	int rightDepth = 0;
	if(node->left) leftDepth = 1 + node->left->depth;
	if(node->right) rightDepth = 1 + node->right->depth;
	
	if(leftDepth - rightDepth > 1) node = rotateRight(node);
	else if(rightDepth - leftDepth > 1) node = rotateLeft(node);
	
	if(node->parent == NULL) tree->root = node;
	
	return result;
}

void * getValue(BST tree, Node node, void * value)
{
	if(!tree || !node) return NULL;
	
	if(tree->compare(value, node->value) > 0) {
		return getValue(tree, node->right, value);
	}
	else if(tree->compare(value, node->value) < 0) {
		return getValue(tree, node->left, value);
	}
	
	return node->value;
}

static int matchValue(BST tree, Node node, void * value)
{
	if(!tree || !node) return 0;
	
	if(tree->compare(value, node->value) > 0) {
		return matchValue(tree, node->right, value);
	}
	else if(tree->compare(value, node->value) < 0) {
		return matchValue(tree, node->left, value);
	}
	
	return 1;
}

Queue getQueueBST(BST tree)
{
    Queue queue = newQueue();
    if(!tree || !tree->root) return queue;
    return getQueueNode(queue, tree->root);
}

static Queue getQueueNode(Queue queue, Node node)
{
    if(!node) return queue;
    addQueue(queue, node->value);
    queue = getQueueNode(queue, node->left);
    queue = getQueueNode(queue, node->right);
    return queue;
}

void freeBST(BST tree)
{
	if(!tree) return;
	freeNode(tree->root);
	free(tree);
}

static void freeNode(Node node)
{
	if(!node) return;
	if(node->left) freeNode(node->left);
	if(node->right) freeNode(node->right);
	free(node);
}

static Node rotateRight(Node node)
{
	if(!node || !node->left) return node;
	
	// Double rotation
	if(node->left->right && !node->left->left) rotateRight(node->left);
	
	Node pivot = node->left;
	
	node->left = pivot->right;
	if(pivot->right) pivot->right->parent = node;
	
	pivot->right = node;
	
	pivot->parent = node->parent;
	
	if(node->parent) {
		if(node->parent->left == node) node->parent->left = pivot;
		else node->parent->right = pivot;
	}
	
	node->parent = pivot;
	
	updateDepth(node);
	
	return pivot;
}

static Node rotateLeft(Node node)
{
	if(!node || !node->right) return node;
	
	// Double rotation
	if(node->right->left && !node->right->right) rotateLeft(node->right);
	
	Node pivot = node->right;
	
	node->right = pivot->left;
	if(pivot->left) pivot->left->parent = node;
	
	pivot->left = node;
	
	pivot->parent = node->parent;
	
	if(node->parent) {
		if(node->parent->left == node) node->parent->left = pivot;
		else node->parent->right = pivot;
	}
	
	node->parent = pivot;
	
	updateDepth(node);
	
	return pivot;
}

static Node minNode(Node node)
{
	if(!node) return NULL;
	
	if(!node->left) return node;
	return minNode(node->left);
}

static Node maxNode(Node node)
{
	if(!node) return NULL;
	
	if(!node->right) return node;
	return maxNode(node->right);
}

static void updateDepth(Node node)
{
	if(!node) return;
	
	if(!node->left && !node->right) {
		if(node->depth != 0) {
			node->depth = 0;
			updateDepth(node->parent);
		}
		return;
	}
	
	int depth = 0;
	
	if(node->left) depth = node->left->depth;
	if(node->right && node->right->depth > depth) depth = node->right->depth;
	
	depth++;

	if(node->depth != depth) {
		node->depth = depth;
		updateDepth(node->parent);
	}
}