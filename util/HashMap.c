#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HashMap.h"
#include "BST.h"

#define DEFAULT_SIZE 16
#define LOAD_FACTOR 0.75
#define SIZE_MULTIPLIER 2
#define TRANSFER_RATE 8

struct HashEntry {
	void * key;
	void * value;
	int (*compare) (void * a, void * b);
};

typedef struct HashEntry * HashEntry;

static HashEntry newHashEntry(void * key, void * value, int (*compare) (void * a, void * b));
static HashEntry getHashEntry(HashMap map, void * key);
static int compareEntries(void * a, void * b);
static unsigned long hashKey(HashMap map, void * key);

static unsigned long stringHash(void * key);
static int stringCompare(void * a, void * b);

static void resize(HashMap map);
static void transferBuckets(HashMap map);

struct HashMap {
	unsigned long (* hash) (void * value);
	int (*compare) (void * a, void * b);
	
	BST * buckets;
	
	unsigned long capacity;
	unsigned long size;
	
	struct HashMap * old;
	int resizing;
	int transfering;
};

HashMap newHashMap(int (*compare) (void * a, void * b), unsigned long (* hash) (void * value))
{
	HashMap map = malloc(sizeof(struct HashMap));
	
	map->hash = hash;
	map->compare = compare;
	
	map->buckets = malloc(sizeof(BST) * DEFAULT_SIZE);
	memset(map->buckets, 0, sizeof(BST) * DEFAULT_SIZE);
	
	map->capacity = DEFAULT_SIZE;
	map->size = 0;
	
	map->old = NULL;
	map->resizing = 0;
	map->transfering = 0;
	
	return map;
}

HashMap newStringHashMap()
{
	return newHashMap(stringCompare, stringHash);
}

int addHashMap(HashMap map, void * key, void * value)
{
	if(!map) return 0;
	if(map->resizing && !map->transfering) transferBuckets(map);
	
	unsigned long hash = hashKey(map, key);
	unsigned long index = hash % map->capacity;
	
	HashEntry entry = newHashEntry(key, value, map->compare);
	
	if(!map->buckets[index])
		map->buckets[index] = newBST(compareEntries);	
	else if(existsBST(map->buckets[index], entry)) {
		removeBST(map->buckets[index], entry);
		map->size--;
	}
	
	addBST(map->buckets[index], entry);
	
	map->size++;
	
	if((double) map->size / map->capacity > LOAD_FACTOR) {
		resize(map);
	}
	
	return 1;
}

void * getHashMap(HashMap map, void * key)
{
	if(!map) return NULL;
	if(map->resizing) transferBuckets(map);
	
	HashEntry entry = getHashEntry(map, key);
	if(!entry) return NULL;
	
	return entry->value;
}

void * getKeyHashMap(HashMap map, void * key)
{
	if(!map) return NULL;
	if(map->resizing) transferBuckets(map);
	
	HashEntry entry = getHashEntry(map, key);
	if(!entry) return NULL;
	
	return entry->key;
}

void * removeHashMap(HashMap map, void * key)
{
	if(!map) return NULL;
	if(map->resizing) transferBuckets(map);
	
	unsigned long hash = hashKey(map, key);
	unsigned long index = hash % map->capacity;
	
	HashEntry entry = newHashEntry(key, NULL, map->compare);
	
	void * result = NULL;
	
	if(map->buckets[index]) {
		result = removeBST(map->buckets[index], entry);
		if(result) map->size--;
	}
	
	if(!result && map->resizing) {
		result = removeHashMap(map->old, key);
	}
	
	return result;
}

int existsHashMap(HashMap map, void * key)
{
	if(!map) return 0;
	if(map->resizing) transferBuckets(map);
	
	unsigned long hash = hashKey(map, key);
	unsigned long index = hash % map->capacity;
	
	if(!map->buckets[index]) return 0;
	
	HashEntry entry = newHashEntry(key, NULL, map->compare);
	
	int exists = existsBST(map->buckets[index], entry);
	
	if(!exists && map->old) return existsHashMap(map->old, key);
	return exists;
}

int sizeHashMap(HashMap map)
{
	if(map->resizing) transferBuckets(map);
	
	if(map->resizing) return map->size + map->old->size;
	return map->size;
}

int emptyHashMap(HashMap map)
{
	if(map->resizing) transferBuckets(map);
	
	if(map->resizing && !emptyHashMap(map->old)) return 0;
	return sizeHashMap(map) ? 0 : 1;
}

static void resize(HashMap map)
{
	while(map->resizing) transferBuckets(map);
	
	HashMap old = newHashMap(map->compare, map->hash);
	
	old->buckets = map->buckets;
	old->size = map->size;
	old->capacity = map->capacity;
	
	map->size = 0;
	map->capacity *= SIZE_MULTIPLIER;
	
	map->buckets = malloc(sizeof(BST) * map->capacity);
	memset(map->buckets, 0, sizeof(BST) * map->capacity);
	
	map->old = old;
	map->resizing = 1;
	
	transferBuckets(map);
}

static void transferBuckets(HashMap map)
{
	map->transfering = 1;
	
	HashEntry entry;
	int i, j = 0;
	
	for(i = 0; i < TRANSFER_RATE; i++) {
		
		BST bucket = NULL;
		
		while(!bucket && j < map->old->capacity) {
			bucket = map->old->buckets[j++];
		}
		
		if(!bucket) {
			freeHashMap(map->old);
			map->old = NULL;
			map->resizing = 0;
			map->transfering = 0;
			return;
		}
		
		map->old->buckets[j-1] = NULL;
		
		while(!emptyBST(bucket)) {
			entry = removeBST(bucket, maxBST(bucket));
			addHashMap(map, entry->key, entry->value);
			
			map->old->size--;
			free(entry);
		}
	}
	
	map->transfering = 0;
}

void freeHashMap(HashMap map)
{
	if(!map) return;
	
	int i;
	for(i = 0; i < map->capacity; i++) freeBST(map->buckets[i]);
	
	if(map->resizing) freeHashMap(map->old);
	free(map);
}

static HashEntry newHashEntry(void * key, void * value, int (*compare) (void * a, void * b))
{
	HashEntry entry = malloc(sizeof(struct HashEntry));
	entry->key = key;
	entry->value = value;
	entry->compare = compare;
	
	return entry;
}

static HashEntry getHashEntry(HashMap map, void * key)
{
	if(!map) return NULL;
	
	unsigned long hash = hashKey(map, key);
	unsigned long index = hash % map->capacity;
	
	HashEntry entry = newHashEntry(key, NULL, map->compare);
	
	if(map->buckets[index])
		entry = (HashEntry) getBST(map->buckets[index], entry);
	
	if(!entry && map->resizing) entry = getHashMap(map->old, key);
	
	return entry;
}

static int compareEntries(void * a, void * b)
{
	HashEntry A = (HashEntry) a;
	HashEntry B = (HashEntry) b;
	
	return A->compare(A->key, B->key);
}

static unsigned long hashKey(HashMap map, void * key)
{
	unsigned long hash = map->hash(key);
	
	hash ^= (hash >> 20) ^ (hash >> 12);
	hash ^= (hash >> 7) ^ (hash >> 4);
	
	return hash;
}

static unsigned long stringHash(void * key)
{
	char * str = (char *) key;
	unsigned long hash = 5381;
    unsigned int c;

    while((c = * str++)) hash = ((hash << 5) + hash) + c;

    return hash;
}

static int stringCompare(void * a, void * b)
{
	return(strcmp((char *) a, (char *) b));
}

