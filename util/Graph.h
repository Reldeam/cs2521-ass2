typedef struct Graph * Graph;

Graph newGraph(char * name);

int addGraph(Graph graph, Graph neighbour, void * cost);
Graph removeGraph(Graph graph, char * name);

int existsEdgeGraph(Graph graph, char * name);
Graph getEdgeGraph(Graph graph, char * name);
void * getEdgeCostGraph(Graph graph, char * name);

int sizeGraph(Graph graph);

char * nameGraph(Graph graph);

void freeGraph(Graph graph);





