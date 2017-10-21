/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*
*
* Part 1. B - Inverted Index
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "collection.h"
#include "BST.h"
#include "HashMap.h"
#include "Queue.h"
#include "PriorityQueue.h"

//String compare for PriorityQueue.
static int compare(void * a, void * b);	

int main (int argc, char *argv[])
{
	BST words;
	
	// A PriorityQueue of unique words for easy itteration over the collection.
	PriorityQueue wordList = newPriorityQueue(compare);
	
	// A collection of words and corresponding urls where each key is abort
	// word and the corresponding value is a PriorityQueue (BST) of words.
	HashMap collection = newStringHashMap();
	
	char * url;
	char * word;
	PriorityQueue urls;
	
	Queue queue = getUrls();
	
	// Fill the wordList and collection.
	while(!emptyQueue(queue)) {
		
		url = (char *) nextQueue(queue);
		words = (BST) getUrlWords(url);
		
		while(!emptyBST(words)) {
			word = (char *) removeBST(words, maxBST(words));
			
			// Add to wordList.
			if(!existsPriorityQueue(wordList, word)) addPriorityQueue(wordList, word);
			
			// Add to collection.
			if(existsHashMap(collection, word)) urls = (PriorityQueue) getHashMap(collection, word);
			else urls = newPriorityQueue(compare);
			addPriorityQueue(urls, url);
			addHashMap(collection, word, urls);
		}
		
		freeBST(words);
	}
	
	FILE * file = fopen("invertedIndex.txt", "w");
	
	// Print the collection to invertedIndex.txt.
	while(!emptyPriorityQueue(wordList)) {
		word = (char *) nextPriorityQueue(wordList);
		urls = (PriorityQueue) getHashMap(collection, word);
		
		fprintf(file, "%s ", word);
		
		while(!emptyPriorityQueue(urls)) {
			url = (char *) nextPriorityQueue(urls);
			fprintf(file, "%s", url);
			if(!emptyPriorityQueue(urls)) fprintf(file, " ");
		}
		
		if(!emptyPriorityQueue(wordList)) fprintf(file, "\n");
	}
	
	fclose(file);
	
	return 1;
}

static int compare(void * a, void * b)
{
	return strcmp((char *) b, (char *) a);
}

