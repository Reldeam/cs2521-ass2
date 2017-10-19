#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "collection.h"
#include "util/Queue.h"
#include "util/BST.h"
#include "util/HashMap.h"
#include "util/LinkedList.h"
#include "util/PriorityQueue.h"

struct UrlGraph {
	char * name;
	BST outgoing;
	BST incoming;
    
    double pagerank;
    double newPagerank;
    
    double wIn;
    double wOut;
};

typedef struct UrlGraph * UrlGraph;
static UrlGraph newUrlGraph(char * name);
static int compareUrlGraph(void * a, void * b);
static int compareUrlRank(void * a, void * b);

int main (int argc, char *argv[])
{
    if(argc != 4) return 1;
    
    double d = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIteration = atoi(argv[3]);
    
    
	Queue urlNames = getUrls();
    QueueIterator urlIterator = newQueueIterator(urlNames);
    
    char * urlName;
    
    HashMap urls = newStringHashMap();
    BST outgoingLinks;
    char * outgoingName;
    
    UrlGraph url;
    UrlGraph outgoingUrl;
    UrlGraph incomingUrl;
    
    // Create url graphs and populate incoming and outgoing urls
    while(hasNextQueueIterator(urlIterator)) {
        urlName = (char *) nextQueueIterator(urlIterator);
        
        if(existsHashMap(urls, urlName)) url = getHashMap(urls, urlName);
        else {
            url = newUrlGraph(urlName);
            addHashMap(urls, urlName, url);
        }
        
        outgoingLinks = getUrlLinks(urlName);
        
        while(!emptyBST(outgoingLinks)) {
            outgoingName = removeBST(outgoingLinks, maxBST(outgoingLinks));
            
            if(!existsHashMap(urls, outgoingName)) {
                addHashMap(urls, outgoingName, newUrlGraph(outgoingName));
            }
            
            outgoingUrl = (UrlGraph) getHashMap(urls, outgoingName);
            
            if(url != outgoingUrl && !existsBST(url->outgoing, outgoingUrl))
                addBST(url->outgoing, outgoingUrl);
            
            if(url != outgoingUrl && !existsBST(outgoingUrl->incoming, url))
                addBST(outgoingUrl->incoming, url);
        }
    }
    
    resetQueueIterator(urlIterator);
    
    Queue outgoingUrls;
    Queue incomingUrls;
    
    // Set pagerank to 1/N and wIn and wOut
    while(hasNextQueueIterator(urlIterator)) {
        urlName = (char *) nextQueueIterator(urlIterator);
        
        url = getHashMap(urls, urlName);
        url->pagerank = (double) 1 / sizeHashMap(urls);
        url->newPagerank = url->pagerank;
        
        outgoingUrls = getQueueBST(url->outgoing);
        
        while(!emptyQueue(outgoingUrls)) {
            UrlGraph outgoingUrl = nextQueue(outgoingUrls);
            url->wIn += sizeBST(outgoingUrl->incoming);
            url->wOut += sizeBST(outgoingUrl->outgoing);
            if(!sizeBST(outgoingUrl->outgoing)) url->wOut += 0.5;
        }
    }
    
    resetQueueIterator(urlIterator);
    
    int t = 0;
    
    double sum;
    double wIn;
    double wOut;
    
    double diff = diffPR;
    
    // Ajust pagerank with the algorithm thingy
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
                wIn = (double) sizeBST(url->incoming) / incomingUrl->wIn;
                //wOut = (double) sizeBST(url->outgoing) / incomingUrl->wOut;
                if(!sizeBST(url->outgoing)) wOut = (double) 0.5 / incomingUrl->wOut;
                else wOut = (double) sizeBST(url->outgoing) / incomingUrl->wOut;
                sum += incomingUrl->pagerank * wIn * wOut;
            }
            
            url->newPagerank = ((1-d)  / sizeHashMap(urls)) + d * sum;
            diff += fabs(url->newPagerank - url->pagerank);
        }
        
        resetQueueIterator(urlIterator);
        while(hasNextQueueIterator(urlIterator)) {
            url = getHashMap(urls, nextQueueIterator(urlIterator));
            url->pagerank = url->newPagerank;
        } 
    }
    
    resetQueueIterator(urlIterator);
    
    PriorityQueue queue = newPriorityQueue(compareUrlRank);
    while(hasNextQueueIterator(urlIterator)) {
        urlName = (char *) nextQueueIterator(urlIterator);
        url = getHashMap(urls, urlName);
        addPriorityQueue(queue, url);
    }
    
    FILE * file = fopen("pagerankList.txt", "w");
    
    // Set pagerank to 1/N and wIn and wOut
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
	graph->outgoing = newBST(compareUrlGraph);
	graph->incoming = newBST(compareUrlGraph);
    
    graph->pagerank = 0;
    graph->newPagerank = 0;
    
    graph->wIn = 0;
    graph->wOut = 0;
    
    return graph;
}

static int compareUrlGraph(void * a, void * b)
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