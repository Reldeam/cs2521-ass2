#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "collection.h"
#include "util/BST.h"
#include "util/HashMap.h"
#include "util/Queue.h"
#include "util/PriorityQueue.h"

#define MAX_WORD_SIZE 30

static int compare(void * a, void * b);

int main (int argc, char *argv[])
{
	BST words;
	
	PriorityQueue wordList = newPriorityQueue(compare);
	HashMap collection = newStringHashMap();
	
	char * url;
	char * word;
	PriorityQueue urls;
	
	Queue queue = getUrls();
	
	while(!emptyQueue(queue)) {
		url = (char *) nextQueue(queue);
		words = (BST) getUrlWords(url);
		
		while(!emptyBST(words)) {
			word = (char *) maxBST(words);
			
			removeBST(words, word);
			if(!existsPriorityQueue(wordList, word)) addPriorityQueue(wordList, word);
			
			if(existsHashMap(collection, word)) urls = (PriorityQueue) getHashMap(collection, word);
			else urls = newPriorityQueue(compare);
			
			addPriorityQueue(urls, url);
			addHashMap(collection, word, urls);
		}
	}
	
	FILE * file = fopen("invertedIndex.txt", "w");
	
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

