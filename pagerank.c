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

int main (int argc, char *argv[])
{
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
    while(hasNextQueueIterator(urlIterator)) {
        urlName = (char *) nextQueueIterator(urlIterator);
        url = getHashMap(urls, urlName);
        printf("%s: Outgoing %d, Incoming %d\n", urlName, sizeBST(url->outgoing), sizeBST(url->incoming));
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
        
        // ????? 
        //if(!url->wOut) url->wOut = 0.5;
    }
    
    
    
    
    double wIn;
    double wOut;
    
    resetQueueIterator(urlIterator);
    while(hasNextQueueIterator(urlIterator)) {
        urlName = (char *) nextQueueIterator(urlIterator);
        url = getHashMap(urls, urlName);
        
        incomingUrls = getQueueBST(url->incoming);
        
        while(!emptyQueue(incomingUrls)) {
            incomingUrl = nextQueue(incomingUrls);
            wIn = (double) sizeBST(url->incoming) / incomingUrl->wIn;
            
            if(!sizeBST(url->outgoing)) wOut = (double) 0.5 / incomingUrl->wOut;
            else wOut = (double) sizeBST(url->outgoing) / incomingUrl->wOut;
            
            printf("%s -> %s: IN %f, OUT %f\n", incomingUrl->name, urlName, wIn, wOut);
            printf("%f, %f\n", incomingUrl->wIn, incomingUrl->wOut);
        }
        
    }
    
    
    
    
    int t = 0;
    
    int maxIteration = 1000;
    double diffPR = 0.00001;
    double d = 0.85;
    
    
    double sum;
    
    double diff = diffPR;
    
    while(t < maxIteration && diff >= diffPR) {
        t++;
        diff = 0;
        resetQueueIterator(urlIterator);
        while(hasNextQueueIterator(urlIterator)) {
            url = getHashMap(urls, nextQueueIterator(urlIterator));
            incomingUrls = getQueueBST(url->incoming);
            
            url->pagerank = url->newPagerank;
            
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
    }
    
    
    
    
    
    
    
    
    resetQueueIterator(urlIterator);
    
    double total = 0;
    
    // Set pagerank to 1/N and wIn and wOut
    while(hasNextQueueIterator(urlIterator)) {
        urlName = (char *) nextQueueIterator(urlIterator);
        url = getHashMap(urls, urlName);
        printf("%s: %f\n", urlName, url->pagerank);
        total += url->pagerank;
    }
    
    printf("TOTAL: %.7f\n", total);
    
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
