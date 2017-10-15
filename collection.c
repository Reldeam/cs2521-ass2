#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "collection.h"
#include "util/BST.h"
#include "util/PriorityQueue.h"
#include "util/HashMap.h"

#define MAX_WORD_SIZE 25
#define LOCATION "Sample1"
#define COLLECTION "collection.txt"

static int compare(void * a, void * b)
{
	return strcmp((char *) b, (char *) a);
}

Queue getUrls()
{
	Queue queue = newQueue();
	
	FILE * fp = fopen(LOCATION "/" COLLECTION, "r");
	char * word = malloc(sizeof(char) * MAX_WORD_SIZE);
	
	while (fscanf(fp, "%s", word) != EOF) {
		addQueue(queue, word);
		word = malloc(sizeof(char) * MAX_WORD_SIZE);
	}
	
	return queue;
}

BST getUrlWords(char * url)
{
	char * filename = malloc(sizeof(char) * 100);
	memset(filename, 0, 100);
	
	strcpy(filename, LOCATION);
	strcat(filename, "/");
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
	char * filename = malloc(sizeof(char) * 100);
	memset(filename, 0, 100);
	
	strcpy(filename, LOCATION);
	strcat(filename, "/");
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