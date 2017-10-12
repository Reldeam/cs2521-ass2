#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "util/Queue.h"
#include "collection.h"

#include "util/HashMap.h"

int main()
{
	HashMap map = newStringHashMap();
	
	addHashMap(map, "A", "A");
	addHashMap(map, "B", "B");
	addHashMap(map, "C", "B");
	addHashMap(map, "D", "B");
	addHashMap(map, "E", "B");
	
	printf("%s\n", (char *) getHashMap(map, "C"));
	
	return 1;
}
