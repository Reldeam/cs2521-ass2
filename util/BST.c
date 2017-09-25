#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "BST.h"
#include "Queue.h"

struct BST {
	struct Node * root;
	int size;
	int (* compare) (void * a, void * b);
};

struct Node {
	void * value;
	int balance;
	struct Node * parent;
	struct Node * left;
	struct Node * right;
};

typedef struct Node * Node;

static Node newNode(void * value)
{
	Node node = malloc(sizeof(struct Node));
	
	node->value = value;
	node->balance = 0;
	
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	
	return node;
}

static void deleteTree(Node node)
{
	if(!node) return;
	deleteTree(node->left);
	deleteTree(node->right);
	free(node);
}

static Node getMaxNode(Node node)
{
	if(!node) return node;
	while(node->right) node = node->right;
	return node;
}

static Node getMinNode(Node node)
{
	if(!node) return node;
	while(node->left) node = node->left;
	return node;
}

static void printNode(Node node)
{
	if(!node) return;
	
	printf("%d", node->balance);
	
	if(node->left) {
		printf(" L[");
		printNode(node->left);
		printf("]");
	}
	if(node->right) {
		printf(" R[");
		printNode(node->right);
		printf("]");
	}
}

static Node rotateLeft(Node X, Node Z) 
{
	/*
	Node t23 = Z->left;
	X->right = t23;
	if(t23) t23->parent = X;
	*/
	
	X->right = Z->left;
	if(Z->left) Z->left->parent = X;
	
	Z->left = X;
	X->parent = Z;
	
	if(Z->balance == 0) {
		X->balance = 1;
		Z->balance = -1;
	}
	else {
		X->balance = 0;
		Z->balance = 0;
	}
	
	return Z;
}

static Node rotateRight(Node X, Node Z) 
{
	/*
	Node t23 = Z->right;
	X->left = t23;
	if(t23) t23->parent = X;
	*/
	
	X->left = Z->right;
	if(Z->right) Z->right->parent = X;
	
	Z->right = X;
	X->parent = Z;
	
	if(Z->balance == 0) {
		X->balance = -1;
		Z->balance = 1;
	}
	else {
		X->balance = 0;
		Z->balance = 0;
	}
	
	return Z;
}

static Node rotateLeftRight(Node X, Node Z)
{
	Node Y = Z->right;
	Node t3 = Y->left;
	Z->right = t3;
	if(t3) t3->parent = Z;
	Y->left = Z;
	Z->parent = Y;
	
	Node t2 = Y->right;
	X->left = t2;
	if(t2) t2->parent = X;
	Y->right = X;
	X->parent = Y;
 
     if(Y->balance > 0) {
		X->balance = -1;
		Z->balance = 0;
     } 
	 else if(Y->balance == 0) {
		X->balance = 0;
		Z->balance = 0;
	} 
	else {
		X->balance = 0;
		Z->balance = 1; 
	}
	Y->balance = 0;
 
	return Y;
}

static Node rotateRightLeft(Node X, Node Z)
{
	Node Y = Z->left;
	Node t3 = Y->right;
	Z->left = t3;
	if (t3) t3->parent = Z;
	Y->right = Z;
	Z->parent = Y;
	
	Node t2 = Y->left;
	X->right = t2;
	if (t2) t2->parent = X;
	Y->left = X;
	X->parent = Y;
	
     if(Y->balance > 0) {
		X->balance = -1;
		Z->balance = 0;
     } 
	 else if(Y->balance == 0) {
		X->balance = 0;
		Z->balance = 0;
	} 
	else {
		X->balance = 0;
		Z->balance = 1;
	}
	Y->balance = 0;
 
	return Y; 
}

static void copyNode(Node original, Node copy, Node previous)
{
	if(!original) return;
	
	Node left = NULL;
	Node right = NULL;
	
	if(original->left) left = newNode(original->left->value);
	if(original->right) right = newNode(original->right->value);
	
	if(!copy) copy = newNode(original->value);
	
	copy->left = left;
	copy->right = right;
	
	if(previous) copy->parent = previous;
	
	copyNode(original->left, copy->left, copy);
	copyNode(original->right, copy->right, copy);
}

BST newBST(int (* compare) (void * a, void * b))
{
	BST tree = malloc(sizeof(struct BST));
	
	tree->root = NULL;
	tree->size = 0;
	
	tree->compare = compare;
	
	return tree;
}

BST getCopyOfBST(BST original)
{
	if(!original) return NULL;
	
	BST copy = newBST(original->compare);
	if(!original->root) return copy;
	
	copy->root = newNode(original->root->value);
	
	copyNode(original->root, copy->root, NULL);
	copy->size = original->size;
	
	return copy;
}

void deleteBST(BST tree)
{
	assert(tree);
	deleteTree(tree->root);
	tree->compare = NULL;
	free(tree);
}

void * getValueFromBST(BST tree, void * value)
{
	assert(tree);
	
	Node currentNode = tree->root;
	
	while(currentNode) {
		if(tree->compare(currentNode->value, value) > 0) {
			currentNode = currentNode->left;
		}
		else if(tree->compare(currentNode->value, value) < 0) {
			currentNode = currentNode->right;
		}
		else break;
	}
	
	if(currentNode) {
		return currentNode->value;
	}
	
	return NULL;
}

void * getMaxValueFromBST(BST tree)
{
	assert(tree);
	
	Node node = getMaxNode(tree->root);
	if(node) return node->value;
	else return NULL;
}

void * getMinValueFromBST(BST tree)
{
	assert(tree);
	
	Node node = getMinNode(tree->root);
	if(node) return node->value;
	else return NULL;
}

int insertIntoBST(BST tree, void * value)
{
	assert(tree);
	
	Node node = newNode(value);
	
	if(!tree->root) {
		tree->root = node;
		tree->size++;
		return 1;
	}
	
	Node currentNode = tree->root;
	
	while(1) {
		if(tree->compare(currentNode->value, value) < 0) {
			
			if(currentNode->right) {
				currentNode = currentNode->right;
				continue;
			}
			else {
				currentNode->right = node;
				node->parent = currentNode;
				break;
			}
		}
		else {
			if(currentNode->left) {
				currentNode = currentNode->left;
				continue;
			}
			else {
				currentNode->left = node;
				node->parent = currentNode;
				break;
			}
		}
	}
	
	Node previousNode = node;
	Node nextNode, rotatedNode;
	
	while(currentNode) {
		if(currentNode->right == previousNode) {
			if(currentNode->balance > 0) {
				nextNode = currentNode->parent;
				if(previousNode->balance < 0) {
					rotatedNode = rotateRightLeft(currentNode, previousNode);
				}
				else {
					rotatedNode = rotateLeft(currentNode, previousNode);
				}
			}
			else {
				if(currentNode->balance < 0) {
					currentNode->balance = 0;
					break;
				}
				currentNode->balance = 1;
				previousNode = currentNode;
				currentNode = currentNode->parent;
				continue;
			}
		}
		else {
			if(currentNode->balance < 0) {
				nextNode = currentNode->parent;
				if(previousNode->balance > 0) {
					rotatedNode = rotateLeftRight(currentNode, previousNode);
				}
				else {
					rotatedNode = rotateRight(currentNode, previousNode);
				}
			}
			else {
				if(currentNode->balance > 0) {
					currentNode->balance = 0;
					break;
				}
				currentNode->balance = -1;
				previousNode = currentNode;
				currentNode = currentNode->parent;
				continue;
			}
		}
		
		rotatedNode->parent = nextNode;
		
		if(nextNode) {
			if(currentNode == nextNode->left) {
				nextNode->left = rotatedNode;
			}
			else {
				nextNode->right = rotatedNode;
			}
			break;
		}
		else {
			tree->root = rotatedNode;
			break;
		}
		
		previousNode = currentNode;
		currentNode = currentNode->parent;
	}
	
	tree->size++;
	return 1;
}

int isEmptyBST(BST tree)
{
	assert(tree);
	if(tree->size == 0) return 1;
	return 0;
}

int isInBST(BST tree, void * value)
{
	assert(tree);
	
	Node currentNode = tree->root;
	
	while(currentNode) {
		if(tree->compare(currentNode->value, value) > 0) {
			currentNode = currentNode->left;
			continue;
		}
		else if(tree->compare(currentNode->value, value) < 0) {
			currentNode = currentNode->right;
			continue;
		}
		else return 1;
	}
	
	return 0;
}

void printBST(BST tree)
{
	printNode(tree->root);
	printf("\n");
}

void * removeFromBST(BST tree, void * value)
{
	assert(tree);
	
	Node currentNode = tree->root;
	Node garbage = NULL;
	
	while(currentNode) {
		if(tree->compare(currentNode->value, value) > 0) {
			currentNode = currentNode->left;
			continue;
		}
		else if(tree->compare(currentNode->value, value) < 0) {
			currentNode = currentNode->right;
			continue;
		}
		else {
			garbage = currentNode;
			if(currentNode->left && currentNode->right) {
				garbage = getMaxNode(currentNode);
				currentNode->value = garbage->value;
				
				currentNode = garbage;
			}
			
			if(currentNode->left) {
				currentNode = currentNode->left;
				break;
			}
			else if(currentNode->right) {
				currentNode = currentNode->right;
				break;
			}
			else {
				currentNode = NULL;
				break;
			}
		}
	}
	
	if(!garbage) return NULL;
	
	Node nextNode = garbage->parent;	
	if(currentNode) currentNode->parent = nextNode;
	
	if(nextNode) {
		if(nextNode->left == garbage) {
			nextNode->left = currentNode;
		}
		else {
			nextNode->right = currentNode;
		}
	}
	else {
		tree->root = NULL;
		value = garbage->value;
		free(garbage);
		tree->size = 0;
		return value;
	}
	
	Node previousNode = currentNode;
	currentNode = nextNode;
	Node siblingNode;
	int siblingBalance;
	
	while(currentNode) {
		nextNode = currentNode->parent;
		
		if(!currentNode->left && !currentNode->right) {
			currentNode->balance = 0;
			previousNode = currentNode;
			currentNode = currentNode->parent;
			continue;
		}
		
		if(currentNode->left == previousNode) {
			if(currentNode->balance > 0) {
				siblingNode = currentNode->right;
				siblingBalance = siblingNode->balance;
				if(siblingBalance < 0) {
					previousNode = rotateRightLeft(currentNode, siblingNode);
				}
				else {
					previousNode = rotateLeft(currentNode, siblingNode);
				}
			}
			else {
				if(currentNode->balance == 0) {
					currentNode->balance = 1;
					break;
				}
				previousNode = currentNode;
				previousNode->balance = 0;
				currentNode = currentNode->parent;
				continue;
			}
		}
		else {
			if(currentNode->balance < 0) {
				siblingNode = currentNode->left;
				siblingBalance = siblingNode->balance;
				if(siblingBalance > 0) {
					previousNode = rotateLeftRight(currentNode, siblingNode);
				}
				else {
					previousNode = rotateRight(currentNode, siblingNode);
				}
			}
			else {
				if(currentNode->balance == 0) {
					currentNode->balance = -1;
					break;
				}
				previousNode = currentNode;
				previousNode->balance = 0;
				currentNode = currentNode->parent;
				continue;
			}
		}
		
		previousNode->parent = nextNode;
		if(nextNode) {
			if(nextNode->left == currentNode) {
				nextNode->left = previousNode;
			}
			else {
				nextNode->right = previousNode;
			}
			if(siblingBalance == 0) break;
		}
		else {
			tree->root = previousNode;
			previousNode = currentNode;
			currentNode = currentNode->parent;
			continue;
		}
		
		previousNode = currentNode;
		currentNode = currentNode->parent;
	}
	
	value = garbage->value;
	free(garbage);
	tree->size--;
	return value;
}

int sizeOfBST(BST tree)
{
	assert(tree);
	return tree->size;
}