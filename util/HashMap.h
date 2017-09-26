#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdarg.h>

struct HashEntry {
	void * key;
	void * value;
};

typedef struct HashEntry * HashEntry;

/**
* HashMap runs all operation in constant time. The map starts with able
* capacity of 16 and a load factor of 0.75. The map will automatically
* resize to fit more keys. The map will resize gradually to ensure no
* excessive time increases for any one method call.
*/
typedef struct HashMap * HashMap;

/**
* A map (table) of unique key, value pairs. This means a each unique
* key appears at most once in the HashMap. HashMap allows for
* constant-time performance for all methods.
*/
HashMap newHashMap(int (* hash) (void * key), int (* compare) (void * a, void * b));

HashMap newStringKeyHashMap();

/**
* Gets the value that the given key is mapped to and returns it.
*
* @param HashMap map The map to retrieve the value from.
* @param void * key The key to search with.
*
* @returns void * The value that the key is mapped to or NULL if the
* value does not exist.
*/
void * getFromHashMap(HashMap map, void * key);

/**
* Gets a set containing all the keys contained in the HashMap.
*
* @param HashMap map The map to get the key set from.
* @param void ** keySet An empty array of keys with enough memory
* allocated to it to be able to have all of the keys of the HashMap added
* to it (there is a one-to-one relationship between the number of keys
* in a HashMap and the size of the HashMap).
*/
void getKeySetOfHashMap(HashMap map, void ** keySet);

/**
* Gets a set containing all the values contained in the HashMap.
*
* @param HashMap map The map to get the value set from.
* @param void ** valueSet An empty array of values with ennough
* memory allocated to it to be able to have all of the values of the
* HashMap addd to it (there is a one-to-one relationship between the
* number of values in a HashMap and the size of the HashMap).
*/
void getValueSetOfHashMap(HashMap map, void ** valueSet);

/**
* Whether or not the HashMap contains any key, value pairs.
*
* @param HashMap map The HashMap to compare.
*
* @returns int 1 if the HashMap contains at least one key, value pair,
* 0 otherwise.
*/
int isEmptyHashMap(HashMap map);

/**
* Whether or not a given HashMap contains a given key.
*
* @param HashMap map The HashMap to search.
* @param void * key The key to search for.
*
* @returns int Returns 1 if the key is contained in the given HashMap,
* or 0 otherwise.
*/
int keyIsInHashMap(HashMap map, void * key);

/**
* Associates the specified value with the specified key in this map. If
* the key already exists in the map then the prevous value will be
* overwritten with the new value.
*
* @param HashMap map The HashMap to add the key, value pair to.
* @param void * key The key to be added.
* @param void * value The value to be added.
*/
void putInHashMap(HashMap map, void * key, void * value);

/**
* Removes the value mapped to by the given key and returns it.
*
* @param HashMap map The HashMap to search.
* @param void * key The key to be mapped to the value.
*
* @returns void * The value that is mapped to by the key or NULL if
* no value can be found.
*/
void * removeFromHashMap(HashMap map, void * key);

/**
* Size of the HashMap.
*
* @param HashMap map The HashMap to get the size of.
*
* @returns int The number of key, value pairs contained in the
* HashMap (one for each key, value pair).
*/
int sizeOfHashMap(HashMap map);



#endif