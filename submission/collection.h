/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*
* These are helper functions for retrieving data from the files provided
* such as collection.txt and invertedIndex.txt. This was mainly for
* abstracting such tasks during development as test suits could begin
* located in seperate directions and then switched to by simpling changing
* LOCATION in collection.c.
*/

#ifndef COLLECTION_H
#define COLLECTION_H


#include "BST.h"
#include "PriorityQueue.h"
#include "HashMap.h"
#include "Number.h"

/*
* Creates a queue of all the urls found in collection.txt.
*/
Queue getUrls();

/*
* Creates a BST that contains all the words found in the url file of the
* corresponding url name provided.
*/
BST getUrlWords(char * url);

/*
* Creates a BST that contains all the outgoing urls found in the url file
* of the corresponding url ame provided.
*/
BST getUrlLinks(char * url);

/*
* Generates a HashMap with word->count (char *)->(Integer) pairs. The
* count is the number of times the word appears in the url. Also creates
* a special total words key (" ").
*/
HashMap getUrlWordsFrequency(char * url);

/*
* Creates a HashMap with word->count (char *)->(Integer) pairs. The
* keys are the urls that match the search terms in invertedIndex.txt and
* the values are the number of times the given url was matched.
*/
HashMap getSearchUrls(PriorityQueue words);

/*
* Creates a HashMap with word->count (char *)->(integer) pairs. The
* keys are the search words (terms) and the values are the number of
* urls that contain that search word.
*/
HashMap getUrlCount(PriorityQueue words);

#endif
