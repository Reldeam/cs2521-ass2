/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "collection.h"
#include "BST.h"
#include "PriorityQueue.h"
#include "HashMap.h"
#include "Number.h"

#define MAX_WORD_SIZE 100
#define LOCATION ""
#define COLLECTION "collection.txt"

static int compare(void * a, void * b)
{
	return strcmp((char *) b, (char *) a);
}

Queue getUrls()
{
	Queue queue = newQueue();
	
	FILE * fp = fopen(LOCATION COLLECTION, "r");
	char * word = malloc(sizeof(char) * MAX_WORD_SIZE);
	
	while (fscanf(fp, "%s", word) != EOF) {
		addQueue(queue, word);
		word = malloc(sizeof(char) * MAX_WORD_SIZE);
	}
	
	return queue;
}

BST getUrlWords(char * url)
{
	char * filename = malloc(sizeof(char) * MAX_WORD_SIZE);
	memset(filename, 0, MAX_WORD_SIZE);
	
	strcpy(filename, LOCATION);
	strcat(filename, url);
	strcat(filename, ".txt");
	
	FILE * fp = fopen(filename, "r");
	
	BST words = newBST(compare);
	
	int i;
	
	int startFlag = 0;
	int wordsFlag = 0;
	
	char * word = malloc(sizeof(char) * MAX_WORD_SIZE);
	
	while (fscanf(fp, "%s", word) != EOF) {
		
		if(startFlag && !strcmp(word, "Section-2")) {
			wordsFlag = 1;
			continue;
		}
		else if(!strcmp(word, "#start")) startFlag = 1;
		else startFlag = 0;
		
		if(!wordsFlag) continue;
		if(!strcmp(word, "#end")) break;
		
		// To lowercase
		for(i = 0; word[i]; i++) word[i] = tolower(word[i]);
		
		// Remove punctuation
		if(word[strlen(word) - 1] == '.') word[strlen(word) - 1] = 0;
		else if(word[strlen(word) - 1] == '?') word[strlen(word) - 1] = 0;
		else if(word[strlen(word) - 1] == ';') word[strlen(word) - 1] = 0;
		else if(word[strlen(word) - 1] == ',') word[strlen(word) - 1] = 0;
		
		if(existsBST(words, word)) continue;
		addBST(words, word);
		
		word = malloc(sizeof(char) * MAX_WORD_SIZE);
	}
	
	return words;
}

BST getUrlLinks(char * url)
{
	char * filename = malloc(sizeof(char) * MAX_WORD_SIZE);
	memset(filename, 0, MAX_WORD_SIZE);
	
	strcpy(filename, LOCATION);
	strcat(filename, url);
	strcat(filename, ".txt");
	
	FILE * fp = fopen(filename, "r");
	
	BST urls = newBST(compare);
	
	int urlsFlag = 0;
	
	char * word = malloc(sizeof(char) * MAX_WORD_SIZE);
	
	while (fscanf(fp, "%s", word) != EOF) {
		
		if(!strcmp(word, "Section-1")) {
			urlsFlag = 1;
			continue;
		}
		
		if(!urlsFlag) continue;
		if(!strcmp(word, "#end")) break;
        
		addBST(urls, word);
		
		word = malloc(sizeof(char) * MAX_WORD_SIZE);
	}
	
	return urls;
}

HashMap getUrlWordsFrequency(char * url)
{
	char * filename = malloc(sizeof(char) * MAX_WORD_SIZE);
	memset(filename, 0, MAX_WORD_SIZE);
	
	strcpy(filename, LOCATION);
	strcat(filename, url);
	strcat(filename, ".txt");
	
	FILE * file = fopen(filename, "r");
	
	HashMap words = newStringHashMap();
	
	Integer count;
	int i;
	
	int total = 0;
	
	int startFlag = 0;
	int wordsFlag = 0;
	
	char * word = malloc(sizeof(char) * MAX_WORD_SIZE);
	
	while (fscanf(file, "%s", word) != EOF) {
		
		if(startFlag && !strcmp(word, "Section-2")) {
			wordsFlag = 1;
			continue;
		}
		else if(!strcmp(word, "#start")) startFlag = 1;
		else startFlag = 0;
		
		if(!wordsFlag) continue;
		if(!strcmp(word, "#end")) break;
		
		// To lowercase
		for(i = 0; word[i]; i++) word[i] = tolower(word[i]);
		
		// Remove punctuation
		if(word[strlen(word) - 1] == '.') word[strlen(word) - 1] = 0;
		else if(word[strlen(word) - 1] == '?') word[strlen(word) - 1] = 0;
		else if(word[strlen(word) - 1] == ';') word[strlen(word) - 1] = 0;
		else if(word[strlen(word) - 1] == ',') word[strlen(word) - 1] = 0;
		
		if(!existsHashMap(words, word)) count = newInteger(1);
		else {
			count = getHashMap(words, word);
			addInteger(count, 1);
		}
		
		addHashMap(words, word, count);
		total++;
		word = malloc(sizeof(char) * MAX_WORD_SIZE);
	}
	
	addHashMap(words, " ", newInteger(total));
	
	return words;
}

HashMap getSearchUrls(PriorityQueue words)
{
	HashMap urls = newStringHashMap();
	Integer count;
	
	FILE * file = fopen("invertedIndex.txt", "r");
    
    char * word = malloc(sizeof(char) * MAX_WORD_SIZE);
    char deliminator;
    
    int found = 0; // Boolean flag for finding word
	
	// Search for search terms in invertedIndex.txt
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
        
        if(!existsHashMap(urls, word)) count = newInteger(1);
        else {
            count = getHashMap(urls, word);
            addInteger(count, 1);
        }
		
		addHashMap(urls, word, count);
		
		word = malloc(sizeof(char) * MAX_WORD_SIZE);
        
        if(deliminator == '\n') {
            nextPriorityQueue(words);
			// All the search terms have been found so we can escape early.
            if(emptyPriorityQueue(words)) break;
            found = 0;
        }
    }
	
	fclose(file);
    
	return urls;
}