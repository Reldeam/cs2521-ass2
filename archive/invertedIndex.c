#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "util/BST.h"
#include "util/HashMap.h"
#include "util/Queue.h"
#include "util/PriorityQueue.h"

#define MAX_WORD_SIZE 30

static void getWords(PriorityQueue keys, HashMap words, char * url);
static int compare(void * a, void * b);

int main (int argc, char *argv[])
{
	Queue urlQueue = newQueue();
	
	FILE * fp = fopen("Sample1/collection.txt", "r");
	char * word = malloc(sizeof(char) * MAX_WORD_SIZE);
	
	while (fscanf(fp, "%s", word) != EOF) {
		
		addToQueue(urlQueue, word);
		word = malloc(sizeof(char) * MAX_WORD_SIZE);
	}
	
	PriorityQueue keys = newPriorityQueue(compare);
	HashMap words = newStringKeyHashMap();
	char * url;
	
	while(!isEmptyQueue(urlQueue)) {
		url = nextFromQueue(urlQueue);
		getWords(keys, words, url);
	}
	
	PriorityQueue urls;
	
	while(!isEmptyPriorityQueue(keys)) {
		word = (char *) nextFromPriorityQueue(keys);
		printf("%s", word);
		
		urls = getFromHashMap(words, word);
		
		if(!isEmptyPriorityQueue(urls)) printf(" ");
		
		while(!isEmptyPriorityQueue(urls)) {
			printf("%s", (char *) nextFromPriorityQueue(urls));
			if(!isEmptyPriorityQueue(urls)) printf(" ");
		}
		
		printf("\n");
	}
	
	return 1;
}

static void getWords(PriorityQueue keys, HashMap words, char * url)
{
	char * filename = malloc(sizeof(char) * 100);
	memset(filename, 0, 100);
	
	strcpy(filename, "Sample1/");
	strcat(filename, url);
	strcat(filename, ".txt");
	
	FILE * fp = fopen(filename, "r");
	
	BST seen = newBST(compare);
	PriorityQueue urls;
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
		int i;
		for(i = 0; word[i]; i++) word[i] = tolower(word[i]);
		
		// Remove punctuation
		if(word[strlen(word) - 1] == '.') word[strlen(word) - 1] = 0;
		else if(word[strlen(word) - 1] == '?') word[strlen(word) - 1] = 0;
		else if(word[strlen(word) - 1] == ';') word[strlen(word) - 1] = 0;
		else if(word[strlen(word) - 1] == ',') word[strlen(word) - 1] = 0;
		
		if(isInBST(seen, word)) continue;
		insertIntoBST(seen, word);
		
		if(!keyIsInHashMap(words, word)) {
			addToPriorityQueue(keys, word);
			putInHashMap(words, word, newPriorityQueue(compare));
		}
		
		urls = getFromHashMap(words, word);
		addToPriorityQueue(urls, url);
		
		word = malloc(sizeof(char) * MAX_WORD_SIZE);
	}
}

static int compare(void * a, void * b)
{
	return strcmp((char *) b, (char *) a);
}

