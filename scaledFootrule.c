#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "util/HashMap.h"
#include "util/BST.h"
#include "util/PriorityQueue.h"

struct Double {
    double value;
};

typedef struct Double * Double;

Double newDouble(double value)
{
    Double d = malloc(sizeof(struct Double));
    d->value = value;
    return d;
}

struct Rank {
    char * url;
    double position;
};

typedef struct Rank * Rank;

static Rank newRank(char * url, double position);
static int rankCompare(void * a, void * b);
static int stringCompare(void * a, void * b);
//static int doubleCompare(void * a, void * b);

int main (int argc, char *argv[])
{  
    if(argc < 2) return 1;

    char * word = malloc(sizeof(char) * 100);
    
    HashMap urls = newStringHashMap();
    PriorityQueue keys = newPriorityQueue(stringCompare);
    HashMap lists[argc -1];
    
    FILE * file;
    int i;
    double position;
    
    for(i = 0; i < argc - 1; i++) {
        
        lists[i] = newStringHashMap();
        file = fopen(argv[i+1], "r");
        
        position = 0;
        
        while (fscanf(file, "%s", word) != EOF) {
            addHashMap(lists[i], word, newDouble(position));
            
            if(!existsHashMap(urls, word)) {
                addHashMap(urls, word, NULL);
                addBST(keys, word);
            }
            
            position++;
            word = malloc(sizeof(char) * 100);
        }
        
        fclose(file);
    }
    
    PriorityQueue ranks = newPriorityQueue(rankCompare);
    Rank rank;
    
    while(!emptyPriorityQueue(keys)) {
        word = nextPriorityQueue(keys);
        
        double * positions = malloc(sizeof(double) * sizeHashMap(urls));
        int count = 0; 
        
        for(i = 0; i < argc - 1; i++) {
            if(!existsHashMap(lists[i], word)) continue;
            position = ((Double) getHashMap(lists[i], word))->value;
            position /= (sizeHashMap(lists[i]) - 1);
            position *= sizeHashMap(urls);
            printf("%s: %f\n", word, position);
            positions[count++] = position;
        }
        
        position = 0;
        
        for(i = 0; i < count; i++) {
            position += positions[i];
        }
        
        position /= count;
        
        addHashMap(urls, word, newDouble(position));
        
        rank = newRank(word, position);
        addPriorityQueue(ranks, rank);
    }
    
    printf("\n");
    
    while(!emptyPriorityQueue(ranks)) {
        rank = nextPriorityQueue(ranks);
        printf("%s: %f\n", rank->url, rank->position);
    }
    
    return 1;
}

static Rank newRank(char * url, double position)
{
    Rank rank = malloc(sizeof(struct Rank));
    rank->url = malloc(sizeof(char) * strlen(url));
    strcpy(rank->url, url);
    rank->position = position;
    
    return rank;
}

static int rankCompare(void * a, void * b)
{
    Rank A = (Rank) a;
    Rank B = (Rank) b;
    
    if(A->position - B->position < 0) return 1;
    if(A->position - B->position > 0) return -1;
    return 0;
}

static int stringCompare(void * a, void * b)
{
    return strcmp((char *) a, (char *) b);
}

/*
static int doubleCompare(void * a, void * b)
{
    Double A = (Double) a;
    Double B = (Double) b;
    return A->value - B->value;
}
*/

