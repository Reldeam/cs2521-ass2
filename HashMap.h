/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*
* A HashMap ADT (Hash Table, Hash, Associative Array, Dictionary, etc)
* 
* Uses AVL Trees as buckets. Implements dynamic resizing when load
* factor is expeded. Resizing is distributed over multiple method calls
* to maintain operation time O(1) at the cost of increasing space
* complexity to O(2n) where n is the capacity of the HashMap.
*
* Note: All functions have time complexity of O(1) unless otherwise stated.
*/

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "BST.h"
#include "Number.h"

typedef struct HashMap * HashMap;

/*
* Creates a new HashMap that compares keys using the provided compare
* function and hashes keys by using the provided hash function.
*/
HashMap newHashMap(int (*compare) (void * a, void * b), unsigned long (* hash) (void * value));

/*
* Having a string as a key is very common so this helper function was created
* that automatically assumes keys are string.
*/
HashMap newStringHashMap();

/*
* Adds the key->value pair to the HashMap, if the key already exists then
* the old value is overwritten with the new value.
*/
int addHashMap(HashMap map, void * key, void * value);

/*
* Returns the value that is mapped to by the provided key or NULL if the
* key->value pair does not exist in the HashMap.
*/
void * getHashMap(HashMap map, void * key);

/*
* Returns the key that matches the provided key. It is possible that the
* returned key is different from the key provided if they both match
* when using the provided comparison.
*/
void * getKeyHashMap(HashMap map, void * key);

/*
* Removes the key->value pair from the HashMap and returns the matching
* value if the key->value pair existed.
*/
void * removeHashMap(HashMap map, void * key);

/*
* Checks if the provided key matches a key in the HashMap. Returns 1
* if it does and 0 otherwise.
*/
int existsHashMap(HashMap map, void * key);

/*
* Returns an array of keys that make up the key set for the HashMap.
* Time complexity: O(n)
*/
void ** getKeySetHashMap(HashMap map);

/*
* The current size of the HashMap (i.e. The number of keys currently
* stored in the HashMap).
*/
int sizeHashMap(HashMap map);

/*
* Returns 1 if the HashMap has at least one key and 0 otherwise.
*/
int emptyHashMap(HashMap map);

/*
* Frees the HashMap from memory.
*/
void freeHashMap(HashMap map);

#endif