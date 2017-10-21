/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*
* An AVL Tree ADT that compares values based on the provided compare
* function at creation time.
*/

#ifndef BST_H
#define BST_H

#include <stdlib.h>
#include <stdio.h>

#include "Queue.h"

typedef struct BST * BST;

BST newBST(int (*compare)(void * a, void * b));

// Time complexity: O(log(n))
int addBST(BST tree, void * value);			// Add value to BST

// Time complexity: O(log(n)))
void * removeBST(BST tree, void * value); 	// Remove value from BST

// Time complexity: O(log(n))
void * getBST(BST tree, void * value);		// Returns value if it exits

// Time complexity: O(log(n))
int existsBST(BST tree, void * value);		// Check if value exists in BST

// Time complexity: O(n)
void printBST(BST tree);							// Print entire BST

// Time complexity: O(1)
int sizeBST(BST tree);								// Number of nodes in BST

// Time complexity: O(1)
int emptyBST(BST tree);							// Check if BST is empty

// Time complexity: O(1)
void * rootBST(BST tree);

// Time complexity: O(log(n))
void * maxBST(BST tree);							// Maximum value in BST

// Time complexity: O(log(n))
void * minBST(BST tree);							// Minimum value in BST

// Time complexity: O(n)
Queue getQueueBST(BST tree);

void freeBST(BST tree);

#endif