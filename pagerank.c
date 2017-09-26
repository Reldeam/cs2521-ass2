#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "util/BST.h"
#include "util/HashMap.h"
#include "util/LinkedList.h"

#define MAX_STRING 8
#define URL_PATH "Sample1/"
#define COLLECTION_FILENAME "collection.txt"

//#include "util/PriorityQueue.h"




struct URLSet {
    HashMap incoming;
    HashMap outgoing;
    double pagerank;
    double newPagerank;
	double wInFactor;
	double wOutFactor;
};

typedef struct URLSet * URLSet;

static URLSet newURLSet()
{
    URLSet set = malloc(sizeof(struct URLSet));
    set->incoming = newStringKeyHashMap();
    set->outgoing = newStringKeyHashMap();
    set->pagerank = 0;
    set->newPagerank = 0;    
	set->wInFactor = 0;
	set->wOutFactor = 0;
    return set;
}

int main (int argc, char *argv[])
{
	assert(argc == 4);
	
	double d = atof(argv[1]);
	double diffPR = atof(argv[2]);
	int maxIterations = atoi(argv[3]);
	
	printf("Max iterations: %d\n", maxIterations);
	
	HashMap sets = newStringKeyHashMap();
	URLSet set;
	
	char * url = malloc(sizeof(char) * MAX_STRING);
	
	FILE * fp = fopen(URL_PATH COLLECTION_FILENAME, "r");
	
	while (fscanf(fp, "%s", url) != EOF) {
		set = newURLSet();
		putInHashMap(sets, url, set);
		url = malloc(sizeof(char) * MAX_STRING);
	}
	
	
	char ** urls = malloc(sizeof(char) * sizeOfHashMap(sets));
    getKeySetOfHashMap(sets, (void **) urls);
	
	char * filename = malloc(sizeof(char) * 50);
	char * outgoingURL = malloc(sizeof(char) * 20);
	URLSet outgoingSet, incomingSet;
	
	int i, j;
	
	// Fill sets
	for(i = 0; i < sizeOfHashMap(sets); i++) {
		
		url = urls[i];
		set = (URLSet) getFromHashMap(sets, url);
		
		strcpy(filename, URL_PATH);
        strcat(filename, url);
        strcat(filename, ".txt");
		
		fp = fopen(filename, "r");
        
        while (fscanf(fp, "%s", outgoingURL) != EOF) {
            if (strstr(outgoingURL, "#end") != NULL) break;
            if (strstr(outgoingURL, "url") == NULL) continue;
			
			outgoingSet = getFromHashMap(sets, outgoingURL);
            putInHashMap(set->outgoing, outgoingURL, outgoingSet);
            putInHashMap(outgoingSet->incoming, url, set);
			
			outgoingURL = malloc(sizeof(char) * 20);
        }
		
		set->pagerank = 1.0 / sizeOfHashMap(sets);
		//set->newPagerank = 1.0 / sizeOfHashMap(sets); 
		
        fclose(fp);
	}
	
	char ** outgoingUrls;
	char ** incomingUrls;
	
	// Calculate wInFactor and wOutFactor
	for(i = 0; i < sizeOfHashMap(sets); i++) {
		
		url = urls[i];
		set = (URLSet) getFromHashMap(sets, url);
		
		outgoingUrls = malloc(sizeof(char) * sizeOfHashMap(set->outgoing));
		getKeySetOfHashMap(set->outgoing, (void **) outgoingUrls);
		
		for(j = 0; j < sizeOfHashMap(set->outgoing); j++) {
			outgoingSet = getFromHashMap(set->outgoing, outgoingUrls[j]);
			
			set->wInFactor += sizeOfHashMap(outgoingSet->incoming);
			set->wOutFactor += sizeOfHashMap(outgoingSet->outgoing);
		}
		
		printf("wInFactor for %s: %f\n", url, set->wInFactor);
		printf("wOutFactor for %s: %f\n", url, set->wOutFactor);
		
		if(set->wInFactor > 0) set->wInFactor = 1.0 / set->wInFactor;
		if(set->wOutFactor > 0) set->wOutFactor = 1.0 / set->wOutFactor;
		
		
		
		//free(outgoingUrls);
	}
	
	double dampShift = (1 - d) / sizeOfHashMap(sets);
	
	double diff = diffPR;
	int iteration = 0;
	int sum, wIn, wOut;
	
	// Weighted Page Rank
	while(iteration < maxIterations && diff >= diffPR) {
		
		
		
		for(i = 0; i < sizeOfHashMap(sets); i++) {
			
			url = urls[i];
			set = (URLSet) getFromHashMap(sets, url);
			
			incomingUrls = malloc(sizeof(char) * sizeOfHashMap(set->incoming));
			getKeySetOfHashMap(set->incoming, (void **) incomingUrls);
			
			sum = 0;
			wIn = 0;
			wOut = 0;
			
			for(j = 0; j < sizeOfHashMap(set->incoming); j++) {
				
				incomingSet = getFromHashMap(set->incoming, incomingUrls[j]);
				
				wIn = sizeOfHashMap(set->incoming) * incomingSet->wInFactor;
				wOut = sizeOfHashMap(set->outgoing) * incomingSet->wOutFactor;
				
				sum += incomingSet->pagerank * wIn * wOut;
			}
			
			set->newPagerank = dampShift + d * sum;
			
			printf("New pagerank for %s: %f\n", url, set->newPagerank);
			
			//free(incomingUrls);
		}
		
		diff = 0;
		
		for(i = 0; i < sizeOfHashMap(sets); i++) {
			url = urls[i];
			set = (URLSet) getFromHashMap(sets, url);
			
			diff += abs(set->newPagerank - set->pagerank);
			set->pagerank = set->newPagerank;
		}
		
		iteration++;
	}
	
	for(i = 0; i < sizeOfHashMap(sets); i++) {
		url = urls[i];
		set = (URLSet) getFromHashMap(sets, url);
		printf("Page rank of %s: %f\n", url, set->pagerank);
	}
	
    return 0;
}
