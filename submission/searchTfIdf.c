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
* Part 2. - Content-based Search Engine
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#include "collection.h"
#include "BST.h"
#include "HashMap.h"
#include "Queue.h"
#include "PriorityQueue.h"

#define MAX_RESULTS 30

struct Result {
	char * url;
	int count;
	double rank;
};

typedef struct Result * Result;

static Result newResult(char * url);
static int resultCompare(void * a, void * b);
static int stringCompare(void * a, void * b);

int main (int argc, char *argv[])
{
	Queue terms = newQueue();
	Queue urls = getUrls();
	
	PriorityQueue termsTemp = newPriorityQueue(stringCompare);
	
    int i, n;
    
	// Convert search terms to lowercase and load into words.
    for(i = 1; i < argc; i++) {
		n = 0;
		while(argv[i][n] != '\0') {
			argv[i][n] = tolower(argv[i][n]);
			n++;
		}
        addPriorityQueue(termsTemp, argv[i]);
		addQueue(terms, argv[i]);
    }
	
	/* Generates HashMap with search terms as keys and the number of
	urls that contain the term as values. */
	HashMap urlCounts = getUrlCount(termsTemp);
	
	// For ease of iteration.
	QueueIterator urlIterator = newQueueIterator(urls);
	QueueIterator termsIterator = newQueueIterator(terms);
	
	char * url;
	char * term;
	HashMap wordCounts;
	
	double tf;
	double idf;
	
	Result result;
	
	PriorityQueue results = newPriorityQueue(resultCompare);
	
	/* Create a priority queue full of results. Each result is a url with its
	corresponding number of matches and rank. */
	while(hasNextQueueIterator(urlIterator)) {
		url = nextQueueIterator(urlIterator);
		
		/* Generates a HashMap with words contained in the given url as
		keys and their corrensponding frequency as values. Also contains
		a special ' ' key that maps to the total number of words in the url.*/
		wordCounts = getUrlWordsFrequency(url);
		
		result = newResult(url);
		
		while(hasNextQueueIterator(termsIterator)) {
			term = nextQueueIterator(termsIterator);
			
			// Calculate tf id values.
			tf = (double) getInteger(getHashMap(wordCounts, term)) / getInteger(getHashMap(wordCounts, " "));
			idf = log10((double) sizeQueue(urls) /  getInteger(getHashMap(urlCounts, term)));
			
			result->rank += tf*idf;
			
			if(existsHashMap(wordCounts, term)) result->count++;
		}
		
		resetQueueIterator(termsIterator);
		
		addPriorityQueue(results, result);
	}
	
	int count = 0;
	
	// Print out the urls in order (max 30)
	while(count++ < MAX_RESULTS && !emptyPriorityQueue(results)) {
		result = nextPriorityQueue(results);
		printf("%s %.6f\n", result->url, result->rank);
	}
	
	return 1;
}

static Result newResult(char * url)
{
	Result result = malloc(sizeof(struct Result));
	result->url = url;
	result->count = 0;
	result->rank = 0;
	return result;
}

static int resultCompare(void * a, void * b)
{
	Result A = (Result) a;
	Result B = (Result) b;
	
	int result = A->count - B->count;
	if(result != 0) return result;
	
	if(A->rank - B->rank > 0) return 1;
	if(A->rank - B->rank < 0) return -1;
	
	return strcmp(A->url, B->url);
}

static int stringCompare(void * a, void * b)
{
	return strcmp((char *) b, (char *) a);
}