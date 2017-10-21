/*
* COMP2521 - Data Structures and Algorithms
* Assignment 2 - Simple Search Engines
*
* Group:	TwoBrothers
*
* Partners:	Kurt Banwell-Pachernegg	(Z5022859)
*				Sam Eager 						(Z3414861)
*
*
* Part 3. - Hybrid/Meta Search Engine using Rank Aggregation
*
* We have chosen to use the Hungarian Method to allocate urls to
* positions on the list. By making the task allocation cost equal the
* partial cost of the scaled-footrule distance for a single url (key) then
* once all keys have been allocated to a position the resultant sum of
* costs equals the entire scaled-footrule distance for all urls over all
* lists.
*
* This method has time complexity class of O(n^4). For more information
* please see https://en.wikipedia.org/wiki/Hungarian_algorithm
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "Number.h"
#include "HashMap.h"
#include "BST.h"
#include "PriorityQueue.h"
#include "Queue.h"

#define MAX_WORD_SIZE 100

struct Item {
   char * url;
   double rank;
};

typedef struct Item * Item;

static Item newItem(char * url, double rank);
static int itemCompare(void * a, void * b);
static int stringCompare(void * a, void * b);
static double distance(char * url, int numLists, HashMap * lists, int position, int numPositions);
static int allAssigned(int size, int * assignments);
static int * assign(int size, double matrix[size][size], int * currentAssignments);
static int reduceMatrix(int size, double matrix[size][size]);

int main (int argc, char *argv[])
{
    if(argc < 2) {
		fprintf(stderr, "There must be at least one file name provided as an argument.\n");
		return 0;
	}
	
	/****************** HUNGARIAN METHOD ******************/
	
	/* STEP 0: POPULATE LISTS
	*
	* Scan through each file provided as argumennts. For earch file create
	* a HashMap that has url -> position (char * -> Integer) pairs.
	*
	* Also add a url to a binary search tree of urls if it has never been
	* seen before in any of the lists (this will form the key set).
	* (From now on a 'key' / 'assignment' refers to a url).
	*/
    
	/* An intersection (unique set) of urls populated from all lists of
	urls provided as arguments. */
    BST keys = newBST(stringCompare);
	
	// Each list has a HashMap of url (char *) -> position (Integer) pairs.
    HashMap lists[argc - 1];
    
    HashMap list;
    FILE * file;
    
    int i, j;
    int position;
    
    char * url = malloc(sizeof(char) * MAX_WORD_SIZE);
    
	/* Populates keys and lists from the files that match the file paths
	provided as arguments. */
    for(i = 1; i < argc; i ++) {
        list = newStringHashMap();
        lists[i-1] = list;
        
        file = fopen(argv[i], "r");
		
		if(!file) continue;
		
        position = 1;
        
		/* Scans through the file word by word, assuming that each word
		is a unique url. The url is added to the current list's corresponding
		HashMap. If the url has never been seen before in any of the lists
		it is added to keys. */
        while (fscanf(file, "%s", url) != EOF) {
            addHashMap(list, url, newInteger(position++));
            if(!existsBST(keys, url)) addBST(keys, url);
            url = malloc(sizeof(char) * MAX_WORD_SIZE);
        }
        
        fclose(file);
    }
	
	if(!sizeBST(keys)) {
		fprintf(stderr, "Either all provided files did not exist or were empty.\n");
		return 0;
	}
	
	/* STEP 1: CREATE COST MATRIX
	* Generate a cost n*n matrix, where n is the number of keys/positions,
	* the rows are keys (urls) and the columns are positions.
	*
	* Each element of the matrix is the cost of key(j) to be assigned to
	* position(j). The cost is the sum of scaled-footrule distances for
	* all lists that contain key(j) (Essential the cost contribution of
	* key(j) if it were to have the final position(j)).
	*/
    
    int size = sizeBST(keys);
    char * row[size];
    int column[size];
    double matrix[size][size];
    
    for(i = 0; i < size; i++) {
        row[i] = removeBST(keys, rootBST(keys));
        for(j = 0; j < size; j++) {
            column[j] = j + 1;
            matrix[i][j] = distance(row[i], argc-1, lists, column[j], size);
        }
    }
    
    double minimum;
    
	/* STEP 2: ROW REDUCTION
	*
	* Subtract the minimum value of each row from each row.
	*
	* For example: The row [5, 2, 3] becomes [3, 0, 1] as 2 is the
	* minimum for that row. This will guarantee there is at least one
	* zero in every row.
	*/
	
    // Subtract minimum of each row from all the entries in that row
    for(i = 0; i < size; i++) {
        minimum = matrix[i][0];
        for(j = 1; j < size; j++) if(matrix[i][j] < minimum) minimum = matrix[i][j];
        for(j = 0; j < size; j++) matrix[i][j] -= minimum;
    }
    
	/* This checks if just doing step 3 was enough to create an assignment
	of positions by checking if a zero can be assigned to each row to a
	unique position. */
    int currentAssignments[size];
    memset(currentAssignments, -1, sizeof(int) * size);
    int * assignments = assign(size, matrix, currentAssignments);
    
	/* STEP 3: COLUMN REDUCTION
	*
	* Subtract the minimum value of each column from each column.
	*
	* The same idea as step 3 except applied to columns, this will ensure
	* that all columns have at least one zero.
	*/
	
	// No assignment could be made do step 3.
    if(!allAssigned(size, assignments)) {
		// Subtract minimum of each column from all the entries in that column
        for(j = 0; j < size; j++) {
            minimum = matrix[0][j];
            for(i = 1; i < size; i++) if(matrix[i][j] < minimum) minimum = matrix[i][j];
            for(i = 0; i < size; i++) matrix[i][j] -= minimum;
        }
		
		// Check again if assignment can be made.
		memset(currentAssignments, -1, sizeof(int) * size);
		assignments = assign(size, matrix, currentAssignments);
    }
    
	/* STEP 4: REDUCTION OF MATRIX
	*
	* Cover minimum number of columns and rows to 'hide' all zeros. Then
	* subtract the minimum of the uncovered values ensuring the generation
	* of at least one new zero and add the minimum to values that are
	* covered at the intersection of a covered row and covered column.
	*
	* This method has been proven to generate a matrix that can be assigned
	* within O(n^2). For more information see 'Hungarian Method' Wikipedia 
	*/
	
	// Reduce matrix until all keys can be assigned positions.
    while(!allAssigned(size, assignments)) {
		
		for(i = 0; i < size; i++) {
			for(j = 0; j < size; j++) {
				printf("%f ", matrix[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		
        reduceMatrix(size, matrix);
        memset(currentAssignments, -1, sizeof(int) * size);
        assignments = assign(size, matrix, currentAssignments);
    }
    
	/**************** END HUNGARIAN METHOD ****************/
	
	/* An Item holds the keys name, position and cost. The queue will
	automatically order each entry by position. */
    PriorityQueue queue = newPriorityQueue(itemCompare);
    Item item;
    
    double cost = 0;
    
	// Create items from the matrix and add them to the priority queue.
	// Also, sum the total cost at this point.
    for(i = 0; i < size; i++) {
        cost += distance(row[i], argc-1, lists, assignments[i]+1, size);
        item = newItem(row[i], assignments[i]);
        addPriorityQueue(queue, item);
    }
	
	// Print the total cost.
    printf("%.6f\n", cost);
    
	// Print all the urls in order.
    while(!emptyPriorityQueue(queue)) {
        item = nextPriorityQueue(queue);
        printf("%s\n", item->url);
    }
    
    return 1;
}

static Item newItem(char * url, double rank)
{
    Item item = malloc(sizeof(struct Item));
    item->url = url;
    item->rank = rank;
    return item;
}

static int itemCompare(void * a, void * b)
{
    return ((Item) b)->rank - ((Item) a)->rank;
}

static int stringCompare(void * a, void * b)
{
    return strcmp((char *) a, (char *) b);
}

/* 
* Returns the cost (scaled-footrule distance) of a single key (url) to a
* single final position. The cost is the sum of the scaled-footrule distance
* for all lists that contain the given key. 
* 
* char * url				key
* int numLists		Total number of lists
* HashMap lists		An array of HashMaps with key->position pairs
* int numPositions	The total number of possible positions
*/ 
static double distance(char * url, int numLists, HashMap * lists, int position, int numPositions)
{
    double sum = 0;
    
    int i;
    for(i = 0; i < numLists; i++) {
        if(!existsHashMap(lists[i], url)) continue;
        int initalPos = getInteger(getHashMap(lists[i], url));
        sum += fabs(((double) initalPos / sizeHashMap(lists[i])) - ((double) position / numPositions));
    }
    
    return sum;
}

/*
* Simply checks that a given array of int does not contain a -1. A -1
* in an assignment array indicates a key that was not assigned a
* position.
*
* int size					Size of array (number of elements)
* int * assignments		Array of assignment positions (-1 indicates no
* assignment)
*/
static int allAssigned(int size, int * assignments)
{
    int i;
    for(i = 0; i < size; i++) if(assignments[i] == -1) return 0;
    return 1;
}

/*
* Attempts to create an assignment of keys to positions so that no key
* shares the same position. A key can be assigned to a position if a 
* key has zero cost at the respective position.
*
* int size				Size of matrix (size * size)
* double matrix[][]	A matrix of assignment costs
* int * currentAssignments	An array of assignments where some
* assignments may have already been allocated, these assignments can
* not be changed
*/
static int * assign(int size, double matrix[size][size], int * currentAssignments)
{
    int i, j, k;
    int columnAssigned;
    
    int * assignments = malloc(sizeof(int) * size);
    for(i = 0; i < size; i++) assignments[i] = currentAssignments[i];
    
    for(i = 0; i < size; i++) {
        if(assignments[i] != -1) continue; 
        for(j = 0; j < size; j++) {
            if(matrix[i][j] == 0) {
				
				// Check that column isn't already assigned
                columnAssigned = 0;
                for(k = 0; k < size; k++) if(assignments[k] == j) {
                    columnAssigned = 1;
                    break;
                }
                if(columnAssigned) continue;
				
				// Lock assignment for (i, j) and test assignments for other
				// positions.
                assignments[i] = j;
                int * result = assign(size, matrix, assignments);
                
				// If an assignment was made with (i, j) locked then return
				if(allAssigned(size, result)) return result;
                
				// Otherwise the assignment could not work so unlock (i, j)
				free(result);
                assignments[i] = -1;
            }
        }
    }    
    return assignments;
}

/*
* Takes a matrix and finds the minimum number of rows and columns to
* 'cover' so that all zeros are covered. Then subtracts the minimum value
* of all uncovered elements from all uncovered elements and adds this
* minimum to any elements that are covered by a covered row and abort
* covered column.
*
* If the matrix could not be reduced any further than 0 is returned, otherwise
* 1 is returned (A non reducible matrix implies a matrix that can be assigned).
*
* int size				Size of matrix (size*size)
* double matrix[][]	Matrix to 'reduce'
*/
static int reduceMatrix(int size, double matrix[size][size])
{
	//Used to mark the covered columns, 0 = uncovered, 1 = covered
    int columns[size];
    memset(columns, 0, sizeof(int) * size);
    
	//Used to mark the covered rows, 0 = uncovered, 1 = covered
    int rows[size];
    memset(rows, 0, sizeof(int) * size);
    
    int mask[size][size];
    memset(mask, 0, sizeof(int) * size * size);
        
    int i, j, k;
    
	/* Generates a mask that indicates whether a row or column should
	be covered. By counting the zeros in row i and zeros in column j and
	taking the difference for each x(i, j) any non zero x's will need to be
	covered by either a column or a row. 
	
	If it is positive then it should be covered by a row and if it is negative 
	then it should be covered by a column.*/
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            for(k = 0; k < size; k++)  {
                if(!matrix[i][k]) mask[i][j]++;
                if(!matrix[k][j]) mask[i][j]--;
            }
            if(mask[i][j] > 0) rows[i] = 1;
            else if(mask[i][j] < 0) columns[j] = 1;
        }
    }  
    
    int min = -1;
    
	// Finds the minimum value of all uncovered elements.
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(!rows[i] && !columns[j] && (matrix[i][j] < min || min == -1)) {
                min = matrix[i][j];
            }
        }
    }
    
    if(min == -1) return 0;
    
	/* Subtracts the minimum value from all uncovered elements and adds
	it to any elements covered by a row and a column. */
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(rows[i] && columns[j]) matrix[i][j] += min;
            if(!rows[i] && !columns[j]) matrix[i][j] -= min;
        }
    }
    
    return 1;
}

