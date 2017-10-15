//COMP2521 - Assignment 2 - Part 2
//To Do:
    //Edit path for reading "inverted.txt" etc.


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#include "util/BST.h"
#include "util/HashMap.h"
#include "util/Queue.h"
#include "util/PriorityQueue.h"

#define MAX_STRING 50
#define URL_PATH "Sample1/"

//Creating a struct to hold urlName, urlPath and an array of doubles to hold information

struct Node {
    char *urlName;
    char *urlPath;
    double *values;
};

typedef struct Node * Node;

static Node newNode(char *urlName, char *urlPath, double *values);
void reformatString(char *src, char *dst);
static int compare(void * a, void * b);

int main (int argc, char *argv[])
{

    assert(argc > 1);
    
//------------totalNumbers URLS + copy urls into Array-------------//        
    
    double totalNumberURLS = 0;
    char * urls = malloc(sizeof(char) * MAX_STRING);

    FILE *fp;
      
    fp = fopen(URL_PATH "collection.txt", "r");
    
    char *URLsArray[MAX_STRING];
    int k = 0;

    //Scans through the 'collection.txt' from part1 to:
        //1. Copy all URLs into an Array of strings so that can open files
        //2. Calculate total number of URLs

    while (fscanf(fp, "%s", urls) != EOF) {
        URLsArray[(int)totalNumberURLS] = malloc((strlen(urls) + strlen(URL_PATH) + strlen(".txt")) + 1);
        strcpy(URLsArray[(int)totalNumberURLS],URL_PATH);
        strcat(urls,".txt");
        strcat(URLsArray[(int)totalNumberURLS],urls);
        totalNumberURLS++;
    }   

    fclose(fp);
    
    int i = 1;
    
//------------IDF Calculation-------------//    
    
    double count = 0;
    int breaker = 0;
    char * urls2 = malloc(sizeof(char) * MAX_STRING);
    
    double idfTfArray[(int)totalNumberURLS+1][argc+2];
    //idfTfArray Structure
    
    /*
            totalWordcound  iniqueSearchWords  TfIdf   argv[1]Tf    argv[2]Tf    ... 
        URL1
        URL2
        URL3
        ...
        Idf val
    
    */
    
    memset(idfTfArray,0,sizeof(idfTfArray)); 
    
    while (i < argc) {
        fp = fopen(URL_PATH "invertedIndex.txt", "r");
        while (fscanf(fp, "%s", urls2) != EOF) { 
            //If breaker == 1, count urls on line until next string that doesn't contain url
            if (breaker == 1) {
                //check: string contains 'url' and a digit (ie. so cURL doesn't get picked as url), if not break
                if (strstr(urls2, "url0") == NULL &&
                    strstr(urls2, "url1") == NULL &&
                    strstr(urls2, "url2") == NULL &&
                    strstr(urls2, "url3") == NULL &&
                    strstr(urls2, "url4") == NULL &&
                    strstr(urls2, "url5") == NULL &&
                    strstr(urls2, "url6") == NULL && 
                    strstr(urls2, "url7") == NULL && 
                    strstr(urls2, "url8") == NULL && 
                    strstr(urls2, "url9") == NULL) {
                    break;
                }
                count++;
            }
            //If reaches string that matches word provided, set breaker = 1
            if (strcasecmp(urls2, argv[i]) == 0) {
                breaker = 1;
            }

        }
        //idf calculation for each word - only calculate if word is actually found, otherwise 0
        if (count > 0) {
            idfTfArray[(int)totalNumberURLS][i+2] = log10(totalNumberURLS/count);        
        }
        i++;
        count = 0;
        breaker = 0;
        fclose(fp);                             
    }
   
//------------TF-------------//        

    int totalWordsInUrl = 0;
    char * urls3 = malloc(sizeof(char) * MAX_STRING); 
    char * dummyUrl = malloc(sizeof(char) * MAX_STRING); 
    int j = 0;
 
  
    for (i = 0; i < totalNumberURLS; i++) {
  
        //Going through Array of saved urls
        fp = fopen(URLsArray[i], "r");
        
        while (fscanf(fp, "%s", dummyUrl) != EOF) {
            //Remove any punctuation etc.
            reformatString(dummyUrl, urls3);
            if (strcmp(urls3, "#start") != 0 && strcmp(urls3, "Section-1") != 0 && strcmp(urls3, "#end") != 0 && strcmp(urls3, "Section-2") != 0) {
                //Scanning through arguments and adding them to relavant array position
                for (k = 1; k < argc; k++) {
                    if (strcasecmp(argv[k],urls3) == 0) {
                        idfTfArray[i][k+2]++;
                    }
                }
                totalWordsInUrl++;            
            }
        }
        
        fclose(fp);
        //Total number of words in the doc
        idfTfArray[i][0] = totalWordsInUrl;
        totalWordsInUrl = 0;
                
        //Calculating tf for all search words in this particular url i
        for (j = 1; j < argc; j++) {
            idfTfArray[i][j+2] = idfTfArray[i][j+2]/idfTfArray[i][0];
        }        
    } 
    
    //Calculating Tf-Idf
    
    double TfIdf = 0;
    double numSearchWordsInUrl = 0;
    
    //Iterate through all urls
    for (i = 0; i < totalNumberURLS; i++) {
        TfIdf = 0;
        numSearchWordsInUrl = 0;
        
        //iterate through all search terms for this url
        for (k = 1; k < argc; k++) {
            //If word was found (ie. a positive Tf value) then add one to totalNumberWords found
            if (idfTfArray[i][k+2] > 0) {
                numSearchWordsInUrl++;
            }
            //              Tf * Idf            
            TfIdf = TfIdf + (idfTfArray[i][k+2]*idfTfArray[(int)totalNumberURLS][k+2]);
        }
        idfTfArray[i][2] = numSearchWordsInUrl;
        idfTfArray[i][1] = TfIdf;
    }      
      
//------------PRINTING OUT RESULTS-------------//        

    printf("URLS               |");
    printf("%s|","#words");
    
    for (i = 1; i < argc + 2; i++) {
        if (i == 1) {
            printf("TfIdf|");
        } else if (i == 2) {
            printf("#words|");            
        } else {
            printf("a%d-Tf|",i-2);
        }
    }
    
    printf("\n");
    
    for (i = 0; i < totalNumberURLS + 1; i++) {
        if (i == totalNumberURLS) {
            printf("Idf ==             |");            
        } else {
            printf("%s  |", URLsArray[i]);        
        }

        for (k = 0; k < argc + 2; k++) {
            printf("%.3f|", idfTfArray[i][k]);            
        }

        printf("\n");
    }    
    
    PriorityQueue q = newPriorityQueue(compare);
    
    Node node;    
    
    for (i = 0; i < totalNumberURLS; i++) {
//        printf("%s %f\n", URLsArray[i], idfTfArray[i][3]);
        addPriorityQueue(q,newNode(URLsArray[i],URLsArray[i],idfTfArray[i]));
    }
   
    i = 0;
    
    while (!emptyPriorityQueue(q)) {
        node = (Node) nextPriorityQueue(q);
        printf("url: %s #words: %f TfIdf: %f i == %d \n", node->urlName, node->values[2], node->values[1], i);
        i++;        
    }
    
    return 0;
}

//Below strips string of punctuation (if any)
//credit to https://stackoverflow.com/questions/1841758/how-to-remove-punctuation-from-a-string-in-c

void reformatString(char *src, char *dst) {
    for (; *src; ++src)
        if (!ispunct((unsigned char) *src))
            *dst++ = tolower((unsigned char) *src);
    *dst = 0;
}

//Compare function that has a two tiered compared
//      1. Compares # of unique search terms that appear in url Node->values[2]
//      2. If equal, then it will compared idTfd value of these urls Node->values[1]

static int compare(void * a, void * b) {
    
    Node n1 = (Node) a;
    Node n2 = (Node) b;

    if (n1->values[2] == n2->values[2]) {
        if(n1->values[1] - n2->values[1] > 0) return 1;
        else if(n1->values[1] - n2->values[1] < 0) return -1;
        else return 0;
    }
    else if(n1->values[2] - n2->values[2] > 0) return 1;
    else return -1;
}

static Node newNode(char *urlName, char *urlPath, double *values) {
    
    Node node = malloc(sizeof(struct Node));
    
    node->urlName = urlName;
    node->urlPath = urlPath;
    node->values = values;
    
    return node;
}
