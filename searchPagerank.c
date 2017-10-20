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
* Part 1. C - Search Engine
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "collection.h"
#include "BST.h"
#include "HashMap.h"
#include "PriorityQueue.h"

#define MAX_WORD_SIZE 100
#define MAX_RESULTS 30

struct Match {
    char * url;
    int count;
    double pagerank;
};

typedef struct Match * Match;

static Match newMatch(char * url);
static int matchCompare(void * a, void * b);
static int stringCompare(void * a, void * b);

int main (int argc, char * argv[])
{
	/* A Priority Queue is used to load the search terms into so that the 
	terms can be itterated over in alphabetical order. This means we only 
	need to read through invertedIndex.txt once to get all the urls and 
	we can also escape early if all the words have been found. */
    PriorityQueue words = newPriorityQueue(stringCompare);
	
    int i, n;
    
	// Convert search terms to lowercase and load into words.
    for(i = 1; i < argc; i++) {
		n = 0;
		while(argv[i][n] != '\0') {
			argv[i][n] = tolower(argv[i][n]);
			n++;
		}
        addPriorityQueue(words, argv[i]);
    }
	
	/* Gets a HashMap with the url name as the key and the
	corresponding number of matching terms from the given Priority Queue
	or words. 
	
	i.e. If the queue has the words 'mars' and 'planet' and url23
	contains both those words then 'url23' will map to 2. If url34 contains
	'mars' but does not contain 'planet' then 'url34' will map to 1. */ 
	HashMap urls = getSearchUrls(words);
	
	// This will be all the urls that contain one or more of the search terms.
    char ** keys = (char **) getKeySetHashMap(urls);
    
	/* This priority queue compares matches (each match is a url that
	matched at least one of the search terms and also contains the number
	of matches as well as the url's pagerank) using the comparison specs
	given in Part 1. C of the assignment.*/
    PriorityQueue matches = newPriorityQueue(matchCompare);
	
	Match match;
	
	/* Fills the priority queue with the matching set of unique urls, their
	number of matches for this search and their corresponding pagerank. */
	for(i = 0; i < sizeHashMap(urls); i++) {
		match = newMatch(keys[i]);
		match->count = getInteger(getHashMap(urls, keys[i]));
		addPriorityQueue(matches, match);
	}
    
    int count = 0; // Counter for MAX_RESULTS
    
	// Prints out the urls in the correct order.
    while(!emptyPriorityQueue(matches)) {
        match = nextPriorityQueue(matches);
        printf("%s\n", match->url);
        if(++count == MAX_RESULTS) break;
    }
    
    return 1;
}

static Match newMatch(char * url)
{
    Match match = malloc(sizeof(struct Match));
    
    match->url = malloc(sizeof(char) * strlen(url));
    strcpy(match->url, url);
    
    match->count = 1;
    match->pagerank = 0;
    
    FILE * file = fopen("pagerankList.txt", "r");
    
    char * urlName = malloc(sizeof(char) * MAX_WORD_SIZE);
    int outgoing;
    float pagerank;
    
    while (fscanf(file, "%s %d, %f", urlName, &outgoing, &pagerank) != EOF) {
        urlName[strlen(urlName)-1] = 0; // Remove comma from url name
        if(!strcmp(url, urlName)) {
            match->pagerank = pagerank;
            break;
        }
    }
    
    fclose(file);
    
    return match;
}

static int matchCompare(void * a, void * b)
{
    Match A = (Match) a;
    Match B = (Match) b;
    
    int result = A->count - B->count;
    
    if(!result) {
        if(A->pagerank - B->pagerank > 0) result = 1;
        else if(A->pagerank - B->pagerank < 0) result = -1;
        else result = strcmp(A->url, B->url);
    }
    
    return result;
}

static int stringCompare(void * a, void * b)
{
    return strcmp((char *) b, (char *) a);
}