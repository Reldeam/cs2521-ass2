/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*/

#ifndef BST_H
#define BST_H

#include "Queue.h"

typedef struct BST * BST;

BST newBST(int (*compare)(void * a, void * b));

int addBST(BST tree, void * value);			// Add value to BST
void * removeBST(BST tree, void * value); 	// Remove value from BST

void * getBST(BST tree, void * value);		// Returns value if it exits
int existsBST(BST tree, void * value);		// Check if value exists in BST

void printBST(BST tree);							// Print entire BST

int sizeBST(BST tree);								// Number of nodes in BST
int emptyBST(BST tree);							// Check if BST is empty

void * rootBST(BST tree);
void * maxBST(BST tree);							// Maximum value in BST
void * minBST(BST tree);							// Minimum value in BST

Queue getQueueBST(BST tree);

void freeBST(BST tree);

#endif