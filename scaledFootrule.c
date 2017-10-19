#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "util/HashMap.h"
#include "util/BST.h"
#include "util/PriorityQueue.h"
#include "util/Queue.h"

struct Integer {
    int value;
};

typedef struct Integer * Integer;

struct Item {
   char * url;
   double rank;
};

typedef struct Item * Item;

static Integer newInteger(int value);
static Item newItem(char * url, double rank);
static int itemCompare(void * a, void * b);
static int stringCompare(void * a, void * b);
static double distance(char * url, int numLists, HashMap * lists, int position, int numPositions);
static int allAssigned(int size, int * assignments);
static int * assign(int size, double matrix[size][size], int * currentAssignments);
static int reduceMatrix(int size, double matrix[size][size]);

int main (int argc, char *argv[])
{
    if(argc < 2) return 1;
    
    BST keys = newBST(stringCompare);
    HashMap lists[argc - 1];
    
    HashMap list;
    FILE * file;
    
    int i, j;
    int position;
    
    char * url = malloc(sizeof(char) * 100);
    
    for(i = 1; i < argc; i ++) {
        list = newStringHashMap();
        lists[i-1] = list;
        
        file = fopen(argv[i], "r");
        position = 1;
        
        while (fscanf(file, "%s", url) != EOF) {
            addHashMap(list, url, newInteger(position++));
            if(!existsBST(keys, url)) addBST(keys, url);
            url = malloc(sizeof(char) * 100);
        }
        
        fclose(file);
    }
    
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
    
    // Subtract minimum of each row from all the entries in that row
    for(i = 0; i < size; i++) {
        minimum = matrix[i][0];
        for(j = 1; j < size; j++) if(matrix[i][j] < minimum) minimum = matrix[i][j];
        for(j = 0; j < size; j++) matrix[i][j] -= minimum;
    }
    
    int currentAssignments[size];
    memset(currentAssignments, -1, sizeof(int) * size);
    int * assignments = assign(size, matrix, currentAssignments);
    
    if(!allAssigned(size, assignments)) {
        // Subtract minimum of each column from all the entries in that column
        for(j = 0; j < size; j++) {
            minimum = matrix[0][j];
            for(i = 1; i < size; i++) if(matrix[i][j] < minimum) minimum = matrix[i][j];
            for(i = 0; i < size; i++) matrix[i][j] -= minimum;
        }
    }
    
    memset(currentAssignments, -1, sizeof(int) * size);
    assignments = assign(size, matrix, currentAssignments);
    
    while(!allAssigned(size, assignments)) {
        reduceMatrix(size, matrix);
        memset(currentAssignments, -1, sizeof(int) * size);
        assignments = assign(size, matrix, currentAssignments);
    }
    
    PriorityQueue queue = newPriorityQueue(itemCompare);
    Item item;
    
    double cost = 0;
    
    for(i = 0; i < size; i++) {
        cost += distance(row[i], argc-1, lists, assignments[i]+1, size);
        item = newItem(row[i], assignments[i]);
        addPriorityQueue(queue, item);
    }
    
    printf("%.6f\n", cost);
    
    while(!emptyPriorityQueue(queue)) {
        item = nextPriorityQueue(queue);
        printf("%s\n", item->url);
    }
    
    return 1;
}

static Integer newInteger(int value)
{
    Integer i = malloc(sizeof(struct Integer));
    i->value = value;
    return i;
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

static double distance(char * url, int numLists, HashMap * lists, int position, int numPositions)
{
    double sum = 0;
    
    int i;
    for(i = 0; i < numLists; i++) {
        if(!existsHashMap(lists[i], url)) continue;
        int initalPos = ((Integer) getHashMap(lists[i], url))->value;
        sum += fabs(((double) initalPos / sizeHashMap(lists[i])) - ((double) position / numPositions));
    }
    
    return sum;
}

static int allAssigned(int size, int * assignments)
{
    int i;
    for(i = 0; i < size; i++) if(assignments[i] == -1) return 0;
    return 1;
}

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
                columnAssigned = 0;
                for(k = 0; k < size; k++) if(assignments[k] == j) {
                    columnAssigned = 1;
                    break;
                }
                if(columnAssigned) continue;
                assignments[i] = j;
                int * result = assign(size, matrix, assignments);
                if(allAssigned(size, result)) return result;
                free(result);
                assignments[i] = -1;
            }
        }
    }
    
    return assignments;
}

static int reduceMatrix(int size, double matrix[size][size])
{
    int columns[size];
    memset(columns, 0, sizeof(int) * size);
    
    int rows[size];
    memset(rows, 0, sizeof(int) * size);
    
    int mask[size][size];
    memset(mask, 0, sizeof(int) * size * size);
        
    int i, j, k;
    
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
    
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(!rows[i] && !columns[j] && (matrix[i][j] < min || min == -1)) {
                min = matrix[i][j];
            }
        }
    }
    
    if(min == -1) return 0;
    
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(rows[i] && columns[j]) matrix[i][j] += min;
            if(!rows[i] && !columns[j]) matrix[i][j] -= min;
        }
    }
    
    return 1;
}

