#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "util/BST.h"
#include "util/HashMap.h"
#include "util/LinkedList.h"

#define MAX_STRING 8
#define URL_PATH "Sample1/"

//#include "util/PriorityQueue.h"

struct URLSet {
    HashMap incoming;
    LinkedList outgoing;
    double pagerank;
    double pagerank1;
};

typedef struct URLSet * URLSet;

static URLSet newURLSet()
{
    URLSet set = malloc(sizeof(struct URLSet));
    set->incoming = newHashMap();
    set->outgoing = newLinkedList();
    set->pagerank = 0;
    set->pagerank1 = 0;    
    return set;
}

int main (int argc, char *argv[])
{
    assert(argc == 4);

    //const double d = atof(argv[1]);
    const double diffPR = atof(argv[2]);
    const int maxIterations = atoi(argv[3]);
    FILE *fp;    
    
    char * URL = malloc(sizeof(char) * MAX_STRING);
    
    HashMap URLs = newHashMap();
    
    fp = fopen(URL_PATH "collection.txt", "r");
    URLSet set;

    while (fscanf(fp, "%s", URL) != EOF) {
        set = newURLSet();
        putInHashMap(URLs, (void *) URL, (void *) set);
        URL = malloc(sizeof(char) * MAX_STRING);     
    }
    
    fclose(fp);

    char **keys = malloc(sizeof(char) * sizeOfHashMap(URLs));

    getKeySetOfHashMap(URLs,(void **)keys);
    
    int i = 0;
    
    for (i = 0; i < sizeOfHashMap(URLs); i++) {
        URL = keys[i];
        set = (URLSet) getFromHashMap(URLs, (void *) URL);
        char urlFileName[MAX_STRING + 50];
        memset(urlFileName, 0, sizeof(char) * (MAX_STRING + 50));
        strcpy(urlFileName, URL_PATH);
        strcat(urlFileName, URL);
        strcat(urlFileName, ".txt");
        printf("Name: %s\n", urlFileName);
        fp = fopen(urlFileName, "r");
        
        char * outgoingURL = malloc(sizeof(char) * 20);
        
        while (fscanf(fp, "%s", outgoingURL)) {
            if (strstr(outgoingURL, "#end") == NULL) break;
            if (strstr(outgoingURL, "url") == NULL) continue;
            
            addToLinkedList(set->outgoing, (void *) outgoingURL);
            URLSet outSet = (URLSet) getFromHashMap(URLs, (void *) outgoingURL);
            if(!outSet) {
               outSet = newURLSet();
               putInHashMap(URLs, (void *) outgoingURL, (void *) outSet);
            }
            putInHashMap(outSet->incoming, (void *) URL, set);
        }
   
        set->pagerank = 1/sizeOfHashMap(URLs);
        fclose(fp);
    }   
    
    int iteration = 0;
    int k = 0;
    double pageRankSum = 0;
    
    double diff = diffPR;
    
    //double dampShift = (1-d)/sizeOfHashMap(URLs);
    
    while (iteration < maxIterations && diff >= diffPR) {
        iteration++;
        
        for (i = 0; i < sizeOfHashMap(URLs); i++) {
            URL = keys[i];
            set = (URLSet) getFromHashMap(URLs, (void *) URL);
            pageRankSum = 0;
            for (k = 0; sizeOfHashMap(URLs); k++) {
                
                pageRankSum += set->pagerank;
            }
        }
        
    }
    
    
    return 0;
}
