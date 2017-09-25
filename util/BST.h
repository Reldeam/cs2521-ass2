#ifndef BST_H
#define BST_H

typedef struct BST * BST;

/**
* An AVL Tree (Self-Balancing Binary Search Tree)
*
* @param int (* compare) The compare method that will be used to 
* compare values.
*/
BST newBST(int (* compare) (void * a, void * b));

/**
* Copies the tree. The copied tree shares the same pointer locations for
* the values of each node. It is safe to add/remove/delete the copy 
* without affecting the original tree.
*
* @param BST original The original BST tree to be copied.
*
* @returns BST tree A copy of the original BST.
*/
BST getCopyOfBST(BST original);

/**
* Frees all memory associated with the BST 
* (does not free node values).
*
* @param The BST to delete.
*/
void deleteBST(BST tree);

/**
* Gets the value matching the given value. These two values could be
* different but are equivalent using the given compare() method.
*
* @param BST tree The BST tree to search.
* @param void * value The value to try and match.
*
* @return void * Returns the matching value or NULL.
*/
void * getValueFromBST(BST tree, void * value);

/**
* Gets the maximum value in the BST.
*
* @param BST tree The tree to search.
* 
* @returns void * The largest value in the BST or NULL if the BST is
* empty.
*/
void * getMaxValueFromBST(BST tree);

/**
* Gets the minimum value in the BST.
*
* @param BST tree The tree to search.
* 
* @returns void * The smallest value in the BST or NULL if the BST is
* empty.
*/
void * getMinValueFromBST(BST tree);

/**
* Inserts a new value into the BST according to the compare method.
* Smaller values are placed to the left and larger values are placed
* to the right.
*
* @param BST tree The BST to be inserted into.
* @param void * value The value to be inserted into the BST.
*
* @returns int 1 if the value was successfully inserted into the
* BST, 0 if the value was not able to be compared.
*/
int insertIntoBST(BST tree, void * value);

/**
* Whether or not the BST contains any values.
*
* @param BST tree BST to check.
*
* @returns int 1 if the BST tree has no values, 0 otherwise. 
*/
int isEmptyBST(BST tree);

/**
* Checks whether the given value is in the BST.
*
* @param BST tree The tree to search.
* @param void * value The value to search for.
*
* @returns int 1 if the value is in the BST, 0 otherwise.
*/
int isInBST(BST tree, void * value);

/**
* Prints the BST to the standard output stream.
*
* @param BST tree The BST to print.
*/
void printBST(BST tree);

/**
* Attempts to remove given value from the BST.
*
* @param BST tree The tree to remove the value from.
* @param void * value The value to attempt to remove.
*
* @returns void * The value that was removed.
*/
void * removeFromBST(BST tree, void * value);

/**
* The current number of values stored in the BST.
*
* @param BST tree The tree to get the size of.
*
* @returns int Size of the BST (number of stored values).
*/
int sizeOfBST(BST tree);

#endif