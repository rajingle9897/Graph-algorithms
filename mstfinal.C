#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h> 

struct Edge 
{ 
	int source;
	int dest;
	int weight; 
}; 

struct Graph 
{ 
	int vertices;
	int edges; 
	struct Edge* edge; 
}; 


struct Graph* graphcreate(int vertices, int edges) 
{ 
	struct Graph* graph = new Graph; 
	graph->vertices = vertices; 
	graph->edges = edges; 

	graph->edge = new Edge[edges]; 

	return graph; 
} 


struct subset 
{ 
	int parent; 
	int rank; 
}; 


int find(struct subset subsets[], int i) 
{ 
	if (subsets[i].parent != i) 
		subsets[i].parent = find(subsets, subsets[i].parent); 
	return subsets[i].parent; 
} 

void Union(struct subset subsets[], int x, int y) 
{ 
	int xroot = find(subsets, x); 
	int yroot = find(subsets, y); 

	if (subsets[xroot].rank < subsets[yroot].rank) 
		subsets[xroot].parent = yroot; 
	else if (subsets[xroot].rank > subsets[yroot].rank) 
		subsets[yroot].parent = xroot; 

	else
	{ 
		subsets[yroot].parent = xroot; 
		subsets[xroot].rank++; 
	} 
} 

int compare(const void* a, const void* b) 
{ 
	struct Edge* a1 = (struct Edge*)a; 
	struct Edge* b1 = (struct Edge*)b; 
	return a1->weight > b1->weight; 
} 

void constructMST(struct Graph* graph) 
{ 
	int vertices = graph->vertices; 
	struct Edge result[vertices];  
	int e = 0;  
	int i = 0;  


	qsort(graph->edge, graph->edges, sizeof(graph->edge[0]), compare); 

	struct subset *subsets = (struct subset*)malloc( vertices * sizeof(struct subset)); 


	for (int v = 0; v < vertices; ++v) 
	{ 
		subsets[v].parent = v; 
		subsets[v].rank = 0; 
	} 


	while (e < vertices - 1 && i < graph->edges) 
	{ 
		struct Edge next_edge = graph->edge[i++]; 

		int x = find(subsets, next_edge.source); 
		int y = find(subsets, next_edge.dest); 


		if (x != y) 
		{ 
			result[e++] = next_edge; 
			Union(subsets, x, y); 
		} 

	} 


	printf("Following are the edges in the constructed MST\n"); 
	int totalcost=0;
	for (i = 0; i < e; ++i)
	{
		result[i].source=result[i].source+'a'; 
		result[i].dest=result[i].dest+'a';
		printf("%c -- %c == %d\n", result[i].source, result[i].dest, 
			result[i].weight);
		totalcost=totalcost+result[i].weight;
	}
	printf("Totalcost=%d",totalcost);
	return; 
} 

int main() 
{ 
	clock_t t;
	t = clock();

	int vertices;  
	int edges;  


	int i=0;

	FILE *fp;
	char data[50];
	fp=fopen("ut04.txt","r");
	if(fp==NULL)
	{
		printf("Can't open");
		return 1;  
	}

	char *line;
	char x,y,z;
	line=fgets(data,50,fp);

	int j = 0;
	char *p = strtok (line, " ");
	char *array[3];

	while (p != NULL)
	{
		array[j++] = p;
		p = strtok (NULL, " ");
	}

	vertices=atoi(array[0]);
	edges=atoi(array[1]);
	
	struct Graph* graph = graphcreate(vertices, edges);

	line=fgets(data,50,fp);
	while(line!=NULL)
	{

		char x,y;
		int z;
		int j = 0;
		char *p = strtok (line, " ");
		char *array[3];

		while (p != NULL)
		{
			array[j++] = p;
			p = strtok (NULL, " ");
		}


		x=line[0];
		y=line[2];
		z=atoi(array[2]);

		x=x-'a';
		y=y-'a';

		graph->edge[i].source = x; 
		graph->edge[i].dest = y; 
		graph->edge[i].weight = z;
		

		i++;

		line=fgets(data,50,fp);
	}

	constructMST(graph); 

	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC;
	printf("\nRun time=%f seconds\n", time_taken);

	return 0; 
} 
