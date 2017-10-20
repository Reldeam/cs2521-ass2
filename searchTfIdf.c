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
	/* The search terms are put into a priority queue that orders terms
	alphabetically. This is required as getSearchUrls(terms) will only
	search through invertedIndex.txt for urls once. If terms are not in
	alphabetic order then some terms will be missed as terms is
	consumed by getSearchUrls. */
	PriorityQueue terms = newPriorityQueue(stringCompare);
	
	/* This queue combined with an iterator later on will provide an easy
	way of iterating over the search terms multiple times. */
	Queue keys = newQueue();
	
    int i, n;
    
	// Convert search terms to lowercase and load into words.
    for(i = 1; i < argc; i++) {
		n = 0;
		while(argv[i][n] != '\0') {
			argv[i][n] = tolower(argv[i][n]);
			n++;
		}
        addPriorityQueue(terms, argv[i]);
		addQueue(keys, argv[i]);
    }
	
	/* Builds a HashMap where each key is a url name and the 
	corresponding value is the number of times the words in the url 
	matched a search term. */
	HashMap urlCount = getSearchUrls(terms);
	
	// An array of all the matching url names.
	char ** urls = (char **) getKeySetHashMap(urlCount);
	
	/* This HashMap will be populated with url names as the keys and
	a HashMap as the value. The HashMap value has words that the url 
	contains that matched the search terms as the keys, with each word 
	having a corresponding number of times the word appears in the url 
	as the value. */
	HashMap wordCount = newStringHashMap();

	// Populate the wordCount HashMap.
	for(i = 0; i < sizeHashMap(urlCount); i++)
		addHashMap(wordCount, urls[i], getUrlWordsFrequency(urls[i]));
	
	/* Each matching url will be converted to a Result which contains
	the number of search terms the url matched as well as its tf-idf value.
	
	These results are then put in this priority queue which will order the
	results as per the specifications in Part 2. of the assignment. */
	PriorityQueue results = newPriorityQueue(resultCompare);
	
	double tf;
	double idf;
	
	char * url;
	char * word;
	
	Result result;
	HashMap words;
	int count, totalWords, frequency;
	
	// Reminder: keys are the search terms.
	QueueIterator iterator = newQueueIterator(keys);
	
	/* The results priority queue is populated by iterating over the urls
	and calculating the tf-idf for each one. */
	for(i = 0; i < sizeHashMap(urlCount); i++) {
		
		url = urls[i];
		
		// Number of search terms the url has matched.
		count = getInteger(getHashMap(urlCount, url));
		
		/* The words that the url matched (as keys) with corresponding number of
		times the word appears in the url as the value. */
		words = getHashMap(wordCount, url);
		
		// The total number of words that the url contains.
		// (See collection.h for more information)
		totalWords = getInteger(getHashMap(words, " "));
		
		result = newResult(url);
		result->count = count;
		
		idf = log10((double) sizeHashMap(urlCount) / count);
		
		/* All of the search terms are itterated over and the tf-idf value is
		calculated for any terms that exist in the current url.
		
		This is cheaper than creating a keyset of words for ^most searches.
		
		^ A search with many search terms that has spares results (i.e.
		one url match per term) would make this approach less efficient
		than generating keysets for each word. */
		while(hasNextQueueIterator(iterator)) {
			word = nextQueueIterator(iterator);
			if(!existsHashMap(words, word)) continue;
			frequency = getInteger(getHashMap(words, word));
			
			tf = ((double) frequency / totalWords) * idf;
			result->rank += tf * idf;
		}
		
		resetQueueIterator(iterator);
		
		addPriorityQueue(results, result);
	}
	
	count = 0; // Counter for MAX_RESULTS
	
	// Print out the results in the correct order.
	// (Maximum of 30 results)
	while(!emptyPriorityQueue(results)) {
		result = nextPriorityQueue(results);
		printf("%s %f\n", result->url, result->rank);
		if(++count == MAX_RESULTS) break;
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