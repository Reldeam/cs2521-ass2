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
};

typedef struct URLSet * URLSet;

static URLSet newURLSet()
{
    URLSet set = malloc(sizeof(struct URLSet));
    set->incoming = newStringKeyHashMap();
    set->outgoing = newStringKeyHashMap();
    set->pagerank = 0;
    set->newPagerank = 0;    
    return set;
}

int main (int argc, char *argv[])
{
	assert(argc == 4);
	
	//double d = atof(argv[1]);
	//double diffPR = atof(argv[2]);
	//int maxIterations = atoi(argv[3]);
	
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
	URLSet outgoingSet;
	
	int i;
	
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
			
			printf("URL: %s\n", outgoingURL);
			
			outgoingSet = getFromHashMap(sets, outgoingURL);
            putInHashMap(set->outgoing, outgoingURL, outgoingSet);
            putInHashMap(outgoingSet->incoming, url, set);
			
			outgoingURL = malloc(sizeof(char) * 20);
        }
		
		set->newPagerank = 1.0 / sizeOfHashMap(sets); 
		
        fclose(fp);
	} 
	
	/*
	double dampShift = (1 - d) / sizeOfHashMap(sets);
	
	int diff = difPR;
	int iteration = 0;
	int j, sum;
	
	while(iteration < maxIterations && diff >= diffPR) {
		
		for(i = 0; i < sizeOfHashMap(sets); i++) {
			url = urls[i];
			set = (URLSet) getFromHashMap(sets, url);
			set->pagerank = set->newPagerank;
			
			sum = 0;
			
			for(j = 0; j <) //HERE
		}
		
		iteration++;
	}
	*/
	
    return 0;
}
