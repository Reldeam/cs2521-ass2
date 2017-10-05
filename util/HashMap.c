#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "HashMap.h"
#include "BST.h"

static const unsigned long MAX_CAPACITY = 1 << 31;
static const unsigned long DEFAULT_CAPACITY = 2;

static const int SIZE_MULTIPLIER = 2;
static const float LOAD_FACTOR = 0.75f;
static const int NUM_BUCKET_TRANSFERS = 2;

static HashEntry newHashEntry(void * key, void * value);
static int keyIsInTree(BST tree, void * key);

static unsigned long basicHash(unsigned int hash);
static unsigned long defaultHash(void * key);
static unsigned long stringHash(void * key);

static int defaultCompare(void * a, void * b);
static int stringCompare(void * a, void * b);

static void stopResizing(HashMap map);
static void transferBuckets(HashMap map);
static void resizeHashMap(HashMap map);

struct HashMap {
	
	int (* compare) (void * a, void * b);
	unsigned long (* hash) (void * key);
	
	BST * table;
	unsigned long capacity;
	unsigned long size;
	
	char transfering;
	char resizing;
	
	BST * previousTable;
	unsigned long previousCapacity;
	unsigned long previousSize;
	unsigned long transferIndex;
};

HashMap newHashMap(unsigned long (* hash) (void * key), int (* compare) (void * a, void * b))
{
	HashMap map = malloc(sizeof(struct HashMap));

	if(!compare) map->compare = defaultCompare;
	else map->compare = compare;
	
	if(!hash) map->hash = defaultHash;
	else map->hash = hash;
		
	map->table = malloc(DEFAULT_CAPACITY * sizeof(BST));
	memset(map->table, 0, DEFAULT_CAPACITY * sizeof(BST));
	
	map->capacity = DEFAULT_CAPACITY;
	map->size = 0;
	
	map->transfering = 0;
	map->resizing = 0;
	
	map->previousTable = NULL;
	map->previousCapacity = 0;
	map->previousSize = 0;
	map->transferIndex = 0;
	
	return map;
}

HashMap newStringKeyHashMap()
{
	return newHashMap(stringHash, stringCompare);
}

void * getFromHashMap(HashMap map, void * key)
{
	unsigned long hash = basicHash(map->hash(key));
	unsigned long index = hash % map->capacity;
	
	BST tree = map->table[index];
	
	if(!tree && map->resizing) {
		unsigned long previousIndex = hash % map->previousCapacity;
		tree = map->previousTable[previousIndex];
	}
	
	if(!tree) return NULL;
	
	HashEntry search = newHashEntry(key, NULL);
	HashEntry entry = (HashEntry) getValueFromBST(tree, (void *) search);
	
	if(map->resizing) transferBuckets(map);
	
	if(entry) return entry->value;
	return NULL;
}

void getKeySetOfHashMap(HashMap map, void ** keySet)
{
	assert(map);
	
	unsigned long index = 0;
	
	unsigned long i;
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
	
	unsigned long index = 0;
	
	unsigned long i;
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
	if(map->resizing) transferBuckets(map);
	
	if(map->size == 0 && map->previousSize == 0) return 0;
	return 1;
}

int keyIsInHashMap(HashMap map, void * key)
{
	BST tree;
	unsigned long i;
	
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
	unsigned long hash = basicHash(map->hash(key));
	unsigned long index = hash % map->capacity;
	
	HashEntry entry = newHashEntry(key, value);
	
	// If no bucket for this hash exists, create it.
	if(!map->table[index]) {
		map->table[index] = newBST(map->compare);
	}
	// Remove the current key, value pair if it exists.
	else if(removeFromBST(map->table[index], (void *) entry)) {
		map->size--;
	}
	
	// Remove the current key, value pair if it exists in previous table.
	if(map->resizing) {
		int previousIndex = hash % map->previousCapacity;
		if(map->previousTable[previousIndex] && removeFromBST(map->previousTable[previousIndex], (void *) entry)) {
			map->previousSize--;
		}
	}
	
	insertIntoBST(map->table[index], (void *) entry);
	map->size++;
	
	if(map->resizing && !map->transfering) {
		transferBuckets(map);
	}
	
	if(map->size > (float) map->capacity * LOAD_FACTOR) {
		resizeHashMap(map);
	}
}

void * removeFromHashMap(HashMap map, void * key)
{
	unsigned long hash = basicHash(map->hash(key));
	unsigned long index = hash % map->capacity;
	
	HashEntry search = newHashEntry(key, NULL);
	HashEntry entry = NULL;
	
	BST tree = map->table[index];
	
	if(tree) entry = (HashEntry) removeFromBST(tree, search);
	
	if(!entry && map->resizing) {
		unsigned long previousIndex = index % map->previousCapacity;
		tree = map->previousTable[previousIndex];
		if(tree) entry = (HashEntry) removeFromBST(tree, search);
	}
	
	if(map->resizing) transferBuckets(map);
	return entry;
}

int sizeOfHashMap(HashMap map)
{
	return map->size + map->previousSize;
}

/**********************************************************/
// STATIC METHODS

static HashEntry newHashEntry(void * key, void * value)
{
	HashEntry entry = malloc(sizeof(struct HashEntry));
	
	entry->key = key;
	entry->value = value;
	
	return entry;
}

static int keyIsInTree(BST tree, void * key)
{
	HashEntry entry = newHashEntry(key, NULL);
	if(isInBST(tree, entry)) return 1;
	return 0;
}

static unsigned long basicHash(unsigned int hash)
{
	hash ^= (hash >> 20) ^ (hash >> 12);
	hash ^= (hash >> 7) ^ (hash >> 4);
	
	return hash;
}

static unsigned long defaultHash(void * key)
{
	unsigned long index = (unsigned long) key;
	
	return basicHash(index);
}

static unsigned long stringHash(void * key)
{
	char * str = (char *) key;
	unsigned long hash = 5381;
    unsigned int c;

    while((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
	}

    return hash;
}

static int defaultCompare(void * a, void * b)
{
	HashEntry A = (HashEntry) a;
	HashEntry B = (HashEntry) b;
	
	return defaultHash(A->key) - defaultHash(B->key);
}

static int stringCompare(void * a, void * b)
{
	HashEntry A = (HashEntry) a;
	HashEntry B = (HashEntry) b;
	return(strcmp((char *) A->key, (char *) B->key));
}

static void transferBuckets(HashMap map)
{
	BST tree;
	HashEntry entry;
	unsigned long i;
	
	map->transfering = 1;
	
	for(i = 0; i < NUM_BUCKET_TRANSFERS; i++) {
		
		while(!map->previousTable[map->transferIndex]) {
			map->transferIndex++;
			if(map->transferIndex >= map->previousCapacity) {
				stopResizing(map);
				return;
			}
		}
		
		tree = map->previousTable[map->transferIndex];
		
		while(!isEmptyBST(tree)) {
			entry = (HashEntry) getMaxValueFromBST(tree);
			removeFromBST(tree, (void *) entry);
			printf("Removed from previous map: %s\n", (char *) entry->value);
			putInHashMap(map, entry->key, entry->value);
			map->previousSize--;
		}
		
		deleteBST(tree);
		map->previousTable[map->transferIndex] = NULL;
	}
	
	map->transfering = 0;
}

static void resizeHashMap(HashMap map)
{
	if(map->capacity * SIZE_MULTIPLIER >= MAX_CAPACITY) return;
	
	while(map->resizing) transferBuckets(map);
	
	printf("Starting new resize...\n");
	
	map->previousCapacity = map->capacity;
	map->capacity *= SIZE_MULTIPLIER;
	
	map->previousSize = map->size;
	map->size = 0;
	
	map->previousTable = map->table;
	map->table = malloc(map->capacity * sizeof(BST));
	memset(map->table, 0, map->capacity * sizeof(BST));
	
	map->transferIndex = 0;
	
	map->resizing = 1;
	map->transfering = 0;
}

static void stopResizing(HashMap map)
{
	printf("Finished resize.\n");
	
	free(map->previousTable);
	map->previousTable = NULL;
	map->previousSize = 0;
	
	map->transfering = 0;
	map->resizing = 0;
	
	map->transferIndex = 0;
}