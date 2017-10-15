#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>


#include "util/PriorityQueue.h"

int stringCompare(void * a, void * b)
{
    return strcmp((char *) a, (char *) b);
}

int main()
{
	PriorityQueue queue = newPriorityQueue(stringCompare);
	
    addPriorityQueue(queue, "a");
    
	return 1;
}
