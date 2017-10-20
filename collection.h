/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*/

#ifndef COLLECTION_H
#define COLLECTION_H


#include "BST.h"
#include "PriorityQueue.h"
#include "HashMap.h"
#include "Number.h"

Queue getUrls();

BST getUrlWords(char * url);
BST getUrlLinks(char * url);

HashMap getUrlWordsFrequency(char * url);

HashMap getSearchUrls(PriorityQueue words);

#endif
