//Assignment 2 - Data Structures and Algorithms - COMP2521 ---------------------------------------
//Semester 2, 2017
//Name: TwoBrothers
//October 2017
//Reference/acknowledgement:
        //This work is wholly our own work. I do acknowledge the COMP2521 lecture slides
        //have been used to create the code below.
        //Any other code that is not our own has been referenced appropriately

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#include "collection.h"
#include "BST.h"
#include "HashMap.h"
#include "Queue.h"
#include "PriorityQueue.h"

#define MAX_STRING 64

//Creating a struct to hold urlName, urlPath and an array of doubles to hold information

struct Node {
    char *urlName;
    char *urlPath;
    double *values;
};

typedef struct Node * Node;

static Node newNode(char *urlName, char *urlPath, double *values);
static void reformatString(char *src, char *dst);
static int compare(void * a, void * b);
static int isWord(char * word);

int main (int argc, char *argv[])
{

    assert(argc > 1);
    
//------------totalNumbers URLS + copy urls into Array-------------//        
    
    
    char * word = malloc(sizeof(char) * MAX_STRING);
    
    Queue urlsQueue = getUrls();
    int totalNumberUrls = sizeQueue(urlsQueue);
  
    //Creating an array to hold the list of urls and a list of the url path
    char urlsPathArray[totalNumberUrls][MAX_STRING];
    char urlsArray[totalNumberUrls][MAX_STRING];    
   
    int i;    

    //Iterating through the list of urls and copying them to relevant array
    for (i = 0; i < totalNumberUrls; i++) {
        word = nextQueue(urlsQueue); 
        strcpy(urlsArray[i], word);
        strcat(word,".txt"); 
        strcpy(urlsPathArray[i],word);
    }
    
//---------------------------------------IDF Calculation--------------------------------------//    

        
                                //idfTfArray Structure//
    
    /*
                urlWordcound   urlTfIdf uniqueSearchWords  argv[1]Tf    argv[2]Tf    ... 
        URL1
        URL2
        URL3
        ...
        Idf val
    
    */
    
    double count = 0;
    int breaker = 0;

    //Creating an array of doubles to hold IdfTf data for each search word and url
    //Initialising all elements to 0
    double idfTfArray[totalNumberUrls+1][argc+2];
    memset(idfTfArray,0,sizeof(idfTfArray));     
    
    FILE *fp;
    i = 0;
    
    while (i < argc) {
        //opening invertedIndex to calculate how many urls the particular search words appear in
        fp = fopen("invertedIndex.txt", "r");
        
        while (fscanf(fp, "%s", word) != EOF) { 
            //If breaker == 1, count urls on line (ie. urls that it appears in)
            //until the next non-url string
            
            if (breaker == 1) {
                //If the string is a word (ie. non-url) break out of the while loop
                if (isWord(word)) {
                    break;
                }
                count++;
            }

            //If reaches string that matches search-word provided, set breaker = 1
            if (strcasecmp(word, argv[i]) == 0) {
                breaker = 1;
            }
        }
        
        //IDF calculation for each search word. Note it only calculates if word appeared in invertedIndex.txt
        // idf ("searchTerm = log10('total Number Of Urls' / 'number Of Urls That Term Appears In')
        //Otherwise IDF will remain 0
        
        if (count > 0) {
            idfTfArray[totalNumberUrls][i+2] = log10((double)totalNumberUrls/count);        
        }
        
        i++;
        count = 0;
        breaker = 0;
        fclose(fp);                             
    }
   
//---------------------------------------Tf Calculation--------------------------------------//    

    count = 0;
    int k = 0;    
  
    for (i = 0; i < totalNumberUrls; i++) {
        //Going through Array of saved urls
        fp = fopen(urlsPathArray[i], "r");

        while (fscanf(fp, "%s", word) != EOF) {

            //Remove any punctuation/capitalisation for easy string comparison
            reformatString(word, word);
            if (isWord(word)) {
                //Scanning through search terms and adding the count to relevant url/search term array position
                for (k = 1; k < argc; k++) {
                    if (strcasecmp(argv[k],word) == 0) {
                        idfTfArray[i][k+2]++;
                        break;
                    }
                }
                //Counting total words in url
                idfTfArray[i][0]++;            
            }
        }
        
        fclose(fp);
        
        //iterate through all search terms for this url
        for (k = 1; k < argc; k++) {
            //Calculating Tf for all search words in this particular url
                //Tf = (number of times search term appears in url) / (total words in url)
            idfTfArray[i][k+2] = idfTfArray[i][k+2]/idfTfArray[i][0];

            //If word was found (ie. a positive Tf value for search term) then add one to 
            // Total number of search terms found in this url
            if (idfTfArray[i][k+2] > 0) {
                idfTfArray[i][2]++;
            }

            // TfIdf Calculation for url = Tf * Idf Calculation (for each search term)
            idfTfArray[i][1] = idfTfArray[i][1] + (idfTfArray[i][k+2]*idfTfArray[totalNumberUrls][k+2]);
        }
    } 
      
//-----------------------------------Printing out results----------------------------------//    

    //Initialise a priority queue with a compare function that sorts by
    //  1. number of search words in url, and (if equal)
    //  2. IdfTf value
    
    PriorityQueue q = newPriorityQueue(compare);
    Node node;
    
    for (i = 0; i < totalNumberUrls; i++) {
        addPriorityQueue(q,newNode(urlsArray[i],urlsPathArray[i],idfTfArray[i]));
    }

    i = 0;

    while (!emptyPriorityQueue(q) && i < 30) {
        node = (Node) nextPriorityQueue(q);
        printf("%s  %.6f \n", node->urlName, node->values[1]);                
        i++;
    }
    
    return 0;
}

//Below strips string of punctuation (if any)
//credit to https://stackoverflow.com/questions/1841758/how-to-remove-punctuation-from-a-string-in-c

static void reformatString(char *src, char *dst) {
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

//newNode function that allocates the appropriate amount of memory for the struct
//and subsequently allocates the appropriate values

static Node newNode(char *urlName, char *urlPath, double *values) {
    
    Node node = malloc(sizeof(struct Node));
    
    node->urlName = urlName;
    node->urlPath = urlPath;
    node->values = values;
    
    return node;
}

//A function that checks whether a string is not a url or start/end/section string
//It returns 1 if it is a word, and 0 otherwise

static int isWord(char * word) {
            if (strcmp(word, "start") != 0 &&
                strcmp(word, "section1") != 0 &&
                strcmp(word, "end") != 0 &&
                strcmp(word, "section2") != 0 &&
                strstr(word, "url0") == NULL &&
                strstr(word, "url1") == NULL &&
                strstr(word, "url2") == NULL &&
                strstr(word, "url3") == NULL &&
                strstr(word, "url4") == NULL &&
                strstr(word, "url5") == NULL &&
                strstr(word, "url6") == NULL && 
                strstr(word, "url7") == NULL && 
                strstr(word, "url8") == NULL && 
                strstr(word, "url9") == NULL) {
                return 1;
            } else {
                return 0;
            }
}
  
  //Print url/searchword IdfTf Data
    
/*    
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
    
    for (i = 0; i < totalNumberUrls + 1; i++) {
        if (i == totalNumberUrls) {
            printf("Idf ==             |");            
        } else {
            printf("%s  |", urlsPathArray[i]);        
        }

        for (k = 0; k < argc + 2; k++) {
            printf("%.3f|", idfTfArray[i][k]);            
        }

        printf("\n");
    }    

*/
