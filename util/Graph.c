#include <string.h>
#include "HashMap.h"

static int compare(void * a, void * b);
static unsigned long hash(void * key);

struct Graph {
	char * name;
	HashMap edges; 
};

Graph newGraph(char * name)
{
	Graph graph = malloc(sizeof(struct Graph));
	
	graph->name = name;
	graph->edges = newHashMap(compare, hash);
}

int addGraph(Graph graph, Graph neighbour, void * cost)
{
	return addHashMap(graph, neighbour, cost);
}

Graph removeGraph(Graph graph, char * name)
{
	Graph target = newGraph(name);
	int result = removeHashMap(graph, target);
	freeGraph(target);
	return result;
}

int existsEdgeGraph(Graph graph, char * name)
{
	Graph target = newGraph(name);
	return existsHashMap(graph, target);
}

Graph getEdgeGraph(Graph graph, char * name)
{
	Graph target = newGraph(name);
	Graph result = (Graph) getKeyHashMap(graph->edges, target);
	freeGraph(target);
	return result;
}

void * getEdgeCostGraph(Graph graph, char * name)
{
	Graph target = newGraph(name);
	void * result = getHashMap(graph->edges, target);
	freeGraph(target);
	return result;
}

int sizeGraph(Graph graph)
{
	return sizeHashMap(graph->edges);
}

char * nameGraph(Graph graph)
{
	return graph->name;
}

void freeGraph(Graph graph)
{
	free(graph->name);
	freeHashMap(graph->edges);
	free(graph);
}

static int compare(void * a, void * b)
{
	Graph A = (Graph) a;
	Graph B = (Graph) b;
	
	return strcmp(A->name, B->name);
}

static unsigned long hash(void * key)
{
	Graph graph = (Graph) key;
	char * name = graph->name;
	
	unsigned long hash = 5381;
    unsigned int c;

    while((c = * name++)) hash = ((hash << 5) + hash) + c;
	
    return hash;
}