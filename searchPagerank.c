#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util/BST.h"
#include "util/HashMap.h"
#include "util/PriorityQueue.h"

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
    HashMap urls = newStringHashMap();
    BST keys = newBST(stringCompare);
    
    PriorityQueue words = newPriorityQueue(stringCompare);
    int i;
    
    for(i = 1; i < argc; i++) {
        addPriorityQueue(words, argv[i]);
    }
    
    FILE * file = fopen("invertedIndex.txt", "r");
    
    char * word = malloc(sizeof(char) * 128);
    char deliminator;
    
    Match match;
    
    int found = 0;
    
    while (fscanf(file, "%s%c", word, &deliminator) != EOF) {
       
        if(!found) {
            // Search word does not exist in index of words.
            if(strcmp((char *) peakPriorityQueue(words), word) < 0) {
                nextPriorityQueue(words);
                if(emptyPriorityQueue(words)) break;
            }
            if(!strcmp((char *) peakPriorityQueue(words), word)) found = 1;
            else while(deliminator != '\n') fscanf(file, "%s%c", word, &deliminator);
            continue;
        }
        
        if(!existsHashMap(urls, word)) {
            match = newMatch(word);
            addHashMap(urls, word, match);
            addBST(keys, match);
        }
        else {
            match = getHashMap(urls, word);
            match->count++;;
        }
        
        if(deliminator == '\n') {
            nextPriorityQueue(words);
            if(emptyPriorityQueue(words)) break;
            found = 0;
        }
    }
    
    fclose(file);
    
    PriorityQueue matches = newPriorityQueue(matchCompare);
    
    while(!emptyBST(keys)) {
        match = removeBST(keys, rootBST(keys));
        addPriorityQueue(matches, match);
    }
    
    while(!emptyPriorityQueue(matches)) {
        match = nextPriorityQueue(matches);
        printf("%s\n", match->url);
        //printf("%s %d %f\n", match->url, match->count, match->pagerank);
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
    
    char * word = malloc(sizeof(char) * 100);
    int outgoing;
    float pagerank;
    
    while (fscanf(file, "%s %d, %f", word, &outgoing, &pagerank) != EOF) {
        word[strlen(word)-1] = 0;
        if(!strcmp(url, word)) {
           
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