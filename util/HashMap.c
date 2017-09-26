#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "HashMap.h"
#include "BST.h"

static const int MAX_CAPACITY = 1 << 30;
static const int DEFAULT_CAPACITY = 16;
static const int SIZE_MULTIPLIER = 2;
static const float LOAD_FACTOR = 0.75f;
static const int NUM_BUCKET_TRANSFERS = 8;

struct HashMap {
	
	BST * table;
	int capacity;
	int size;
	
	int copying;
	int resizing;
	
	BST * previousTable;
	int previousCapacity;
	int previousSize;
	int currentRemoveIndex;
};

static int getIndex(void * key)
{
	int index = (int) key;
	
	index ^= (index >> 20) ^ (index >> 12);
	index ^= (index >> 7) ^ (index >> 4);
	
	return index;
}

struct HashEntry {
	void * key;
	void * value;
};

typedef struct HashEntry * HashEntry;

static HashEntry newHashEntry(void * key, void * value)
{
	HashEntry entry = malloc(sizeof(struct HashEntry));
	entry->key = key;
	entry->value = value;
	
	return entry;
}

static int compare(void * a, void * b)
{
	HashEntry A = (HashEntry) a;
	HashEntry B = (HashEntry) b;
	
	return getIndex(A->key) - getIndex(B->key);
}

HashMap newHashMap()
{
	HashMap map = malloc(sizeof(struct HashMap));
	
	map->table = malloc(DEFAULT_CAPACITY * sizeof(BST));
	memset(map->table, 0, DEFAULT_CAPACITY * sizeof(BST));
	
	map->capacity = DEFAULT_CAPACITY;
	map->size = 0;
	
	map->copying = 0;
	map->resizing = 0;
	
	map->previousTable = NULL;
	map->previousCapacity = 0;
	map->previousSize = 0;
	map->currentRemoveIndex = 0;
	
	return map;
}

static void stopResizing(HashMap map)
{
	free(map->previousTable);
	map->previousTable = NULL;
	map->previousSize = 0;
	map->copying = 0;
	map->resizing = 0;
}

static void transferBuckets(HashMap map)
{
	BST tree;
	HashEntry entry;
	int i;
	
	map->copying = 1;
	
	for(i = 0; i < NUM_BUCKET_TRANSFERS; i++) {
		
		while(!map->previousTable[map->currentRemoveIndex]) {
			map->currentRemoveIndex++;
			if(map->currentRemoveIndex >= map->previousCapacity) {
				break;
			}
		}
		
		tree = map->previousTable[map->currentRemoveIndex];
		
		if(!tree) {
			stopResizing(map);
			return;
		}
		
		while(!isEmptyBST(tree)) {
			entry = (HashEntry) getMaxValueFromBST(tree);
			removeFromBST(tree, (void *) entry);
			putInHashMap(map, entry->key, entry->value);
			map->previousSize--;
		}
		
		deleteBST(tree);
		map->previousTable[map->currentRemoveIndex] = NULL;
		
		if(map->previousSize == 0) {
			stopResizing(map);
			return;
		}
	}
	
	map->copying = 0;
}

static void resizeHashMap(HashMap map)
{
	while(map->resizing) transferBuckets(map);
	
	map->previousCapacity = map->capacity;
	map->capacity *= SIZE_MULTIPLIER;
	
	map->previousSize = map->size;
	map->size = 0;
	
	map->previousTable = map->table;
	map->table = malloc(map->capacity * sizeof(BST));
	memset(map->table, 0, map->capacity * sizeof(BST));
	
	map->currentRemoveIndex = 0;
	
	map->resizing = 1;
}

void * getFromHashMap(HashMap map, void * key)
{
	int index = getIndex(key);
	int hash = index % map->capacity;
	
	BST tree = map->table[hash];
	
	if(!tree) return NULL;
	
	HashEntry search = newHashEntry(key, NULL);
	
	HashEntry entry = (HashEntry) getValueFromBST(tree, (void *) search);
	
	if(!entry && map->resizing) {
		hash = index % map->previousCapacity;
		tree = map->previousTable[hash];
		entry = (HashEntry) getValueFromBST(tree, (void *) search);
	}
	
	if(map->resizing) transferBuckets(map);
	
	if(!entry) return NULL;
	
	return entry->value;
}

void getKeySetOfHashMap(HashMap map, void ** keySet)
{
	assert(map);
	
	int index = 0;
	
	int i;
	BST tree;
	HashEntry entry;
	
	for(i = 0; i < map->capacity; i++) {
		tree = map->table[i];
		if(!tree) continue;
		tree = getCopyOfBST(tree);
		while(!isEmptyBST(tree)) {
			entry = (HashEntry) removeFromBST(tree, getMinValueFromBST(tree));
			keySet[index++] = entry->key;
		}
	}
	
	if(map->resizing) {
		for(i = 0; i < map->previousCapacity; i++) {
			tree = map->previousTable[i];
			if(!tree) continue;
			tree = getCopyOfBST(tree);
			while(!isEmptyBST(tree)) {
				entry = (HashEntry) removeFromBST(tree, getMinValueFromBST(tree));
				keySet[index++] = entry->key;
			}
		}
	}
}

void getValueSetOfHashMap(HashMap map, void ** valueSet)
{
	assert(map);
	
	int index = 0;
	
	int i;
	BST tree;
	HashEntry entry;
	
	for(i = 0; i < map->capacity; i++) {
		tree = map->table[i];
		if(!tree) continue;
		tree = getCopyOfBST(tree);
		while(!isEmptyBST(tree)) {
			entry = (HashEntry) removeFromBST(tree, getMinValueFromBST(tree));
			valueSet[index++] = entry->value;
		}
	}
	
	if(map->resizing) {
		for(i = 0; i < map->previousCapacity; i++) {
			tree = map->previousTable[i];
			if(!tree) continue;
			tree = getCopyOfBST(tree);
			while(!isEmptyBST(tree)) {
				entry = (HashEntry) removeFromBST(tree, getMinValueFromBST(tree));
				valueSet[index++] = entry->value;
			}
		}
	}
}

int isEmptyHashMap(HashMap map)
{
	if(map->size == 0 && map->previousSize == 0) return 0;
	return 1;
}

static int keyIsInTree(BST tree, void * key)
{
	HashEntry entry = newHashEntry(key, NULL);
	if(isInBST(tree, entry)) return 1;
	return 0;
}

int keyIsInHashMap(HashMap map, void * key)
{
	BST tree;
	int i;
	
	for(i = 0; i < map->capacity; i++) {
		tree = map->table[i];
		if(tree && keyIsInTree(tree, key)) return 1;
	}
	
	if(!map->resizing) return 0;
	
	for(i = 0; i < map->previousCapacity; i++) {
		tree = map->previousTable[i];
		if(tree && keyIsInTree(tree, key)) return 1;
	}
	
	return 0;
}

void putInHashMap(HashMap map, void * key, void * value)
{
	int index = getIndex(key);
	int hash = index % map->capacity;
	
	HashEntry entry = newHashEntry(key, value);
	
	if(!map->table[hash]) {
		map->table[hash] = newBST(compare);
	}
	
	if(map->table[hash] && removeFromBST(map->table[hash], (void *) entry)) {
		map->size--;
	}
	else if(map->resizing) {
		int previousHash = index % map->previousCapacity;
		if(map->previousTable[previousHash] && removeFromBST(map->previousTable[previousHash], (void *) entry)) {
			map->previousSize--;
		}
	}
	
	insertIntoBST(map->table[hash], (void *) entry);
	map->size++;
	
	if(map->resizing) {
		if(!map->copying) transferBuckets(map);
	}
	else if(map->size > (float) map->capacity * LOAD_FACTOR) {
		resizeHashMap(map);
	}
}

void * removeFromHashMap(HashMap map, void * key)
{
	int index = getIndex(key);
	int hash = index % map->capacity;
	
	BST tree = map->table[hash];
	
	HashEntry search = newHashEntry(key, NULL);
	
	HashEntry entry = (HashEntry) removeFromBST(tree, search);
	
	if(!entry && map->resizing) {
		hash = index % map->previousCapacity;
		tree = map->previousTable[hash];
		if(!tree) return NULL;
		entry = (HashEntry) removeFromBST(tree, search);
		if(entry) map->previousSize--;
	}
	else if(entry) {
		map->size--;
	}
	
	if(map->resizing) {
		transferBuckets(map);
	}
	
	return entry;
}

int sizeOfHashMap(HashMap map)
{
	return map->size + map->previousSize;
}