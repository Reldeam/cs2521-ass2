#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "util/BST.h"
#include "util/HashMap.h"

int main()
{
	
	HashMap map = newStringKeyHashMap();
	
	putInHashMap(map, "a", "Apple");
	putInHashMap(map, "b", "Banana");
	putInHashMap(map, "c", "Carrot");
	putInHashMap(map, "d", "Donut");
	
	putInHashMap(map, "e", "Egg");
	putInHashMap(map, "f", "Flour");
	putInHashMap(map, "g", "Glue");
	putInHashMap(map, "h", "Hat");
	
	printf("Total size of HashMap: %d\n", sizeOfHashMap(map));
	
	printf("A is for: %s\n", (char *) getFromHashMap(map, "a"));
	printf("B is for: %s\n", (char *) getFromHashMap(map, "b"));
	printf("C is for: %s\n", (char *) getFromHashMap(map, "c"));
	printf("D is for: %s\n", (char *) getFromHashMap(map, "d"));
	
	printf("E is for: %s\n", (char *) getFromHashMap(map, "e"));
	printf("F is for: %s\n", (char *) getFromHashMap(map, "f"));
	printf("G is for: %s\n", (char *) getFromHashMap(map, "g"));
	printf("H is for: %s\n", (char *) getFromHashMap(map, "h"));
	
	return 1;
}