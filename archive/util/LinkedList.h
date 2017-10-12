#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct LinkedList * LinkedList;

LinkedList newLinkedList();

/**
* Adds a value to the end of the list 
(is an alias for addToEndOfLinkedList()).
*/
void addToLinkedList(LinkedList list, void * value);

/**
* Adds a value to the front of the list.
*/
void addToFrontOfLinkedList(LinkedList list, void * value);

/**
* Adds a value to the end of the list.
*/
void addToEndOfLinkedList(LinkedList list, void * value);

/**
* Adds a value to a specified index in the list, moving the value that
* is currently at that index to the right.
*
* @returns int Returns 1 if successful, 0 if the pos is invalid (i.e the
* position is less that zero or greater that the size of the list).
*/
int insertIntoLinkedList(LinkedList list, int pos, void * value);

/**
* Removes the value at the front of the list and returns it.
*/
void * removeFirstFromLinkedList(LinkedList list);

/**
* Removes the value from the end of the list and returns it.
*/
void * removeLastFromLinkedList(LinkedList list);

/**
* Removes the value from the list that matches the value given and
* returns it.
*/
void * removeFromLinkedList(LinkedList list, void * value);

/**
* Removes the value from the corresponding pos (index) and returns it.
*/
void * spliceLinkedList(LinkedList list, int pos);

/**
* Wether or not a given value is in the list.
*
* @returns int Returns 1 if the value exists, or 0 otherwise.
*/
int isInLinkedList(LinkedList list, void * value);

/**
* Gets the first value from the list without removing it and returns it.
*/
void * getFirstFromLinkedList(LinkedList list);

/**
* Gets the last value from the list withut removing it and returns it.
*/
void * getLastFromLinkedList(LinkedList list);

/**
* Gets the value corresponding to the given pos (index) without
* removing it and returns it.
*/
void * getFromLinkedList(LinkedList list, int pos);

/**
* Returns the number of values currently stored in the list.
*/
int sizeOfLinkedList(LinkedList list);

/**
* Whether or not the list contains any values.
*
* @returns int Returns 1 if the list contains at least one value, or 0
* otherwise.
*/
int isEmptyLinkedList(LinkedList list);

/**
* Prints the list to standard output, convering all of the values to (int).
*/
void printLinkedList(LinkedList list);

#endif