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
* Part 1. A - Calculate Weighted Pageranks
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "collection.h"
#include "Queue.h"
#include "BST.h"
#include "HashMap.h"
#include "LinkedList.h"
#include "PriorityQueue.h"

/*
* Adjacency list graph implemented with two adjacency lists (incoming
* and outgoing edges) for simplicity.
*/
struct UrlGraph {
	char * name;
    
	BST outgoing;				// URLs that this URL references
	BST incoming;				// URLs that reference this URL
    
    double pagerank;		// Pagerank at t
    double newPagerank;	// Pagerank at t+1
    
    double wIn;				// Denominator of Win(u, v) for u
    double wOut;				// Denominator of Wout(u, v) for u
};

typedef struct UrlGraph * UrlGraph;

static UrlGraph newUrlGraph(char * name);
static int compareUrlName(void * a, void * b);
static int compareUrlRank(void * a, void * b);

int main (int argc, char *argv[])
{
	// Program requires three arguments: d (double), diffPR (double) and
	// maxIteration (int). */
    if(argc != 4) {
		fprintf(stderr, "Requires 3 arguments, %d provided.\n", argc - 1);
		return 0;
	}
    
	// Assign arguments to variables for future use.
    double d = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIteration = atoi(argv[3]);
    
	// Load url names from collection.txt (see collection.h for more information).
	Queue urlNames = getUrls();
	
	// urlIterator will be useful for repeatedly iterating over the urls with
	// a reset time of O(C) (see Queue.h for more information).
    QueueIterator urlIterator = newQueueIterator(urlNames);
    
    char * urlName;
    
	// urls will store all of the vertices for constant time access with the 
	// url name as the key.
    HashMap urls = newStringHashMap();
	
    BST outgoingLinks;
    char * outgoingName;
    
	// Variables for generating the vertices of the graph. Each vertex will
	// have two adjacency lists, one for outgoing urls and one for
	// incoming urls.
    UrlGraph url;
    UrlGraph outgoingUrl;
    UrlGraph incomingUrl;
    
    /* Create url vertices and populate incoming and outgoing urls. The
	url HashMap will hold all url names as keys and their respective
	vertex as a value. 
	
	This HashMap will be kept up to date throughout the population of 
	the graph so that time required to construct the adjacency lists of 
	each vertex is minimised as the respective vertices can be found in 
	O(C) time using the HashMap. */
    while(hasNextQueueIterator(urlIterator)) {
		
		// urlIterator will iterate over the queue of urls (urlNames) that 
		// contains all of the urls from collection.txt.
        urlName = (char *) nextQueueIterator(urlIterator);
        
		/* Checks if the vertex for the respective urlName has already been
		created. If it has been then the vertex is retrieved from the
		HashMap, otherwise a new vertex is created and added to the
		HashMap of vertices.
		
		Even though we iterate over the unique urls once, a vertex may
		have already been created as all outgoing url vertices for each
		vertex must be created at the same time so that the corresponding
		incoming and outgoing lists can be populated synchronously. */
        if(existsHashMap(urls, urlName)) url = getHashMap(urls, urlName);
        else {
            url = newUrlGraph(urlName);
            addHashMap(urls, urlName, url);
        }
        
		// Get the outgoing links from the url text file.
        outgoingLinks = getUrlLinks(urlName);
        
		/* Create the outgoing adjacency list for the current url. Adds url 
		to the incoming adjaceny list of the outgoing url at the same time, 
		thus synchronously updating both adjacency lists without having
		to do multiple iterations over the urls. */
        while(!emptyBST(outgoingLinks)) {
            outgoingName = removeBST(outgoingLinks, maxBST(outgoingLinks));
            
            if(!existsHashMap(urls, outgoingName))
                addHashMap(urls, outgoingName, newUrlGraph(outgoingName));
            
            outgoingUrl = (UrlGraph) getHashMap(urls, outgoingName);
            
			// Add the outgoing url to the outgoing adjacency list of url.
            if(url != outgoingUrl && !existsBST(url->outgoing, outgoingUrl))
                addBST(url->outgoing, outgoingUrl);
            
			// Add url to the incoming adjacency list of the outgoing url.
            if(url != outgoingUrl && !existsBST(outgoingUrl->incoming, url))
                addBST(outgoingUrl->incoming, url);
        }
    }
    
    resetQueueIterator(urlIterator);
    
    Queue outgoingUrls;
    Queue incomingUrls;
    
    /* Set pagerank to 1/N and wIn and wOut. Each vertex holds a wIn
	and wOut value. These values are essentially the denominators of
	the wIn(u, v) and wOut(u, v) functions respectively, where the vertex
	with the corresponding wIn and wOut is u. 
	
	This is possible because wIn(u, v)'s and wOut(u, v)'s denominators are
	not effected by v. Thus when calculating the pagerank, this pre-step
	will save a lot of time as the numerator for these functions can be found
	int contant time and so the calculations for both functions can now
	be done in constant time. */
    while(hasNextQueueIterator(urlIterator)) {
        urlName = (char *) nextQueueIterator(urlIterator);
        
        url = getHashMap(urls, urlName);
		
		// Set both pagerank to 1/N where N is the number of urls.
        url->pagerank = (double) 1 / sizeHashMap(urls);
        
		// Get the outgoing url list as a queue so that it can be itterated over.
        outgoingUrls = getQueueBST(url->outgoing);
        
		/* Iterate over the outgoing links of the current url. wIn and wOut
		can be calculated at the same time so we don't have to iterate over
		the outgoing links twice. */
        while(!emptyQueue(outgoingUrls)) {
            UrlGraph outgoingUrl = nextQueue(outgoingUrls);
			
			/* When the vertices are created wIn and wOut are set to zero
			initally so that's why we can go straight into adding here. */
            url->wIn += sizeBST(outgoingUrl->incoming);
            url->wOut += sizeBST(outgoingUrl->outgoing);
			
			// The outgoing url has no outgoing links of it's own.
			// This is that 0.5 fudge in action.
            if(!sizeBST(outgoingUrl->outgoing)) url->wOut += 0.5;
        }
    }
    
    int t = 0; // Counter for maxIteration
    
    double sum;
    double wIn;
    double wOut;
    
    double diff = diffPR;
    
    // Adjusts pagerank using the provided algorithm.
    while(t < maxIteration && diff >= diffPR) {
        
        t++;
        diff = 0;
        
        resetQueueIterator(urlIterator);
		
        while(hasNextQueueIterator(urlIterator)) {
            url = getHashMap(urls, nextQueueIterator(urlIterator));
            incomingUrls = getQueueBST(url->incoming);
            
            sum = 0;
            
            while(!emptyQueue(incomingUrls)) {
                incomingUrl = nextQueue(incomingUrls);
				
				//wIn calculation
                wIn = (double) sizeBST(url->incoming) / incomingUrl->wIn;
				
				//wOut calculation
                if(!sizeBST(url->outgoing)) wOut = (double) 0.5 / incomingUrl->wOut;
                else wOut = (double) sizeBST(url->outgoing) / incomingUrl->wOut;
				
                sum += incomingUrl->pagerank * wIn * wOut;
            }
            
            url->newPagerank = ((1-d)  / sizeHashMap(urls)) + d * sum;
            diff += fabs(url->newPagerank - url->pagerank);
        }
        
        resetQueueIterator(urlIterator);
		
		// Update pagerank(t) to pagerank(t+1)
        while(hasNextQueueIterator(urlIterator)) {
            url = getHashMap(urls, nextQueueIterator(urlIterator));
            url->pagerank = url->newPagerank;
        } 
    }
    
    resetQueueIterator(urlIterator);
    
    PriorityQueue queue = newPriorityQueue(compareUrlRank);
	
	// Put the urls in pagerank order by building a priority queue.
    while(hasNextQueueIterator(urlIterator)) {
        urlName = (char *) nextQueueIterator(urlIterator);
        url = getHashMap(urls, urlName);
        addPriorityQueue(queue, url);
    }
    
    FILE * file = fopen("pagerankList.txt", "w");
    
    // Print the urls in correct order with number of outgoing links and
	// pagerank to pagerankList.txt.
    while(!emptyPriorityQueue(queue)) {
        url = nextPriorityQueue(queue);
        fprintf(file, "%s, %d, %.7f\n", url->name, sizeBST(url->outgoing), url->pagerank);
    }
    
    fclose(file);
    
    return 0;
}

static UrlGraph newUrlGraph(char * name)
{
	UrlGraph graph = malloc(sizeof(struct UrlGraph));
	
	graph->name = name;
	graph->outgoing = newBST(compareUrlName);
	graph->incoming = newBST(compareUrlName);
    
    graph->pagerank = 0;
    graph->newPagerank = 0;
    
    graph->wIn = 0;
    graph->wOut = 0;
    
    return graph;
}

static int compareUrlName(void * a, void * b)
{
    UrlGraph A = (UrlGraph) a;
    UrlGraph B = (UrlGraph) b;
    
    return(strcmp(A->name, B->name));
}

static int compareUrlRank(void * a, void * b)
{
    UrlGraph A = (UrlGraph) a;
    UrlGraph B = (UrlGraph) b;
    
    if(A->pagerank - B->pagerank > 0) return 1;
    else if(A->pagerank - B->pagerank < 0) return -1;
    return 0;
}