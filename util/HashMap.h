#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct HashMap * HashMap;

HashMap newHashMap(int (*compare) (void * a, void * b), unsigned long (* hash) (void * value));
HashMap newStringHashMap();

int addHashMap(HashMap map, void * key, void * value);
void * getHashMap(HashMap map, void * key);
void * getKeyHashMap(HashMap map, void * key);
void * removeHashMap(HashMap map, void * key);
int existsHashMap(HashMap map, void * key);

int sizeHashMap(HashMap map);
int emptyHashMap(HashMap map);

void freeHashMap(HashMap map);

#endif