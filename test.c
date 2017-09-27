#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "util/BST.h"

static int compare(void * a, void * b)
{
	return strcmp((char *) a, (char *) b);
}

int main()
{
	printf("Begin test...\n");
	
	BST tree = newBST(compare);
	
	insertIntoBST(tree, "mars");
	insertIntoBST(tree, "has");
	insertIntoBST(tree, "long");
	insertIntoBST(tree, "been");
	insertIntoBST(tree, "the");
	insertIntoBST(tree, "subject");
	insertIntoBST(tree, "of");
	insertIntoBST(tree, "human");
	insertIntoBST(tree, "interest");
	insertIntoBST(tree, "early");
	insertIntoBST(tree, "telescopic");
	insertIntoBST(tree, "observations");
	insertIntoBST(tree, "revealed");
	insertIntoBST(tree, "color");
	insertIntoBST(tree, "changes");
	insertIntoBST(tree, "on");
	insertIntoBST(tree, "surface");
	insertIntoBST(tree, "that");
	insertIntoBST(tree, "were");
	insertIntoBST(tree, "attributed");
	insertIntoBST(tree, "to");
	insertIntoBST(tree, "seasonal");
	insertIntoBST(tree, "vegetation");
	insertIntoBST(tree, "and");
	insertIntoBST(tree, "apparent");
	insertIntoBST(tree, "linear");
	
	insertIntoBST(tree, "feastures");
	insertIntoBST(tree, "ascribed");
	insertIntoBST(tree, "intelligent");
	insertIntoBST(tree, "design");
	insertIntoBST(tree, "sun");
	insertIntoBST(tree, "pluto");
	insertIntoBST(tree, "g");
	insertIntoBST(tree, "h");
	insertIntoBST(tree, "i");
	insertIntoBST(tree, "j");
	insertIntoBST(tree, "k");
	insertIntoBST(tree, "l");
	insertIntoBST(tree, "m");
	insertIntoBST(tree, "n");
	insertIntoBST(tree, "o");
	insertIntoBST(tree, "p");
	insertIntoBST(tree, "q");
	insertIntoBST(tree, "r");
	insertIntoBST(tree, "s");
	insertIntoBST(tree, "t");
	insertIntoBST(tree, "u");
	insertIntoBST(tree, "v");
	insertIntoBST(tree, "w");
	insertIntoBST(tree, "x");
	insertIntoBST(tree, "y");
	insertIntoBST(tree, "z");
	
	insertIntoBST(tree, "a");
	insertIntoBST(tree, "b");
	insertIntoBST(tree, "c");
	insertIntoBST(tree, "d");
	insertIntoBST(tree, "e");
	insertIntoBST(tree, "f");
	insertIntoBST(tree, "g");
	insertIntoBST(tree, "h");
	insertIntoBST(tree, "i");
	insertIntoBST(tree, "j");
	insertIntoBST(tree, "k");
	insertIntoBST(tree, "l");
	insertIntoBST(tree, "m");
	insertIntoBST(tree, "n");
	insertIntoBST(tree, "o");
	insertIntoBST(tree, "p");
	insertIntoBST(tree, "q");
	insertIntoBST(tree, "r");
	insertIntoBST(tree, "s");
	insertIntoBST(tree, "t");
	insertIntoBST(tree, "u");
	insertIntoBST(tree, "v");
	insertIntoBST(tree, "w");
	insertIntoBST(tree, "x");
	insertIntoBST(tree, "y");
	insertIntoBST(tree, "z");
	
	printf("Size: %d\n", sizeOfBST(tree));
	
	return 1;
}