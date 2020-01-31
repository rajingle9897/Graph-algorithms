#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct 
{
	int vertex;
	int weight;
} edge1;

typedef struct 
{
	edge1 **edges;
	int elength;
	int esize;
	int distance;
	int prev;
	int visited;
} vertex1;

typedef struct 
{
	vertex1 **vertices;
	int vlen;
	int vsize;
} graph1;

typedef struct 
{
	int *value;
	int *prio;
	int *index;
	int len;
	int size;
} heap1;


void addvertex (graph1 *g, int i) 
{
	if (g->vsize < i + 1) 
	{
		int size = g->vsize * 2 > i ? g->vsize * 2 : i + 4;
		g->vertices = (vertex1 **)realloc(g->vertices, size * sizeof(vertex1 *));
		for (int j = g->vsize; j < size; j++)
			g->vertices[j] = NULL;
		g->vsize = size;
	}
	if (!g->vertices[i]) 
	{
		g->vertices[i] = (vertex1*)calloc(1, sizeof (vertex1));
		g->vlen++;
	}
}

void addedge (graph1 *g, int a, int b, int w) 
{

	addvertex(g, a);
	addvertex(g, b);
	vertex1 *v = g->vertices[a];
	if (v->elength >= v->esize) 
	{
		v->esize = v->esize ? v->esize * 2 : 4;
		v->edges = (edge1 **)realloc(v->edges, v->esize * sizeof (edge1 *));
	}
	edge1 *e = (edge1*)calloc(1, sizeof (edge1));
	e->vertex = b;
	e->weight = w;
	v->edges[v->elength++] = e;
}

heap1 *heapcreate (int n) 
{
	heap1 *h = (heap1*)calloc(1, sizeof (heap1));
	h->value = (int*)calloc(n + 1, sizeof (int));
	h->prio = (int*)calloc(n + 1, sizeof (int));
	h->index = (int*)calloc(n, sizeof (int));
	return h;
}

void pushinheap (heap1 *h, int v, int p) 
{
	int i = h->index[v] == 0 ? ++h->len : h->index[v];
	int j = i / 2;
	while (i > 1) 
	{
		if (h->prio[j] < p)
			break;
		h->value[i] = h->value[j];
		h->prio[i] = h->prio[j];
		h->index[h->value[i]] = i;
		i = j;
		j = j / 2;
	}
	h->value[i] = v;
	h->prio[i] = p;
	h->index[v] = i;
}

int minimum (heap1 *h, int i, int j, int k) 
{
	int m = i;
	if (j <= h->len && h->prio[j] < h->prio[m])
		m = j;
	if (k <= h->len && h->prio[k] < h->prio[m])
		m = k;
	return m;
}

int popfromheap (heap1 *h) 
{
	int v = h->value[1];
	int i = 1;
	while (1) 
	{
		int j = minimum(h, h->len, 2 * i, 2 * i + 1);
		if (j == h->len)
			break;
		h->value[i] = h->value[j];
		h->prio[i] = h->prio[j];
		h->index[h->value[i]] = i;
		i = j;
	}
	h->value[i] = h->value[h->len];
	h->prio[i] = h->prio[h->len];
	h->index[h->value[i]] = i;
	h->len--;
	return v;
}

void dijkstra (graph1 *g, int a, int b) 
{
	int i, j;
	a = a - 'a';
	b = b - 'a';
	for (i = 0; i < g->vlen; i++) 
	{
		vertex1 *v = g->vertices[i];
		v->distance = INT_MAX;
		v->prev = 0;
		v->visited = 0;
	}
	vertex1 *v = g->vertices[a];
	v->distance = 0;
	heap1 *h = heapcreate(g->vlen);
	pushinheap(h, a, v->distance);
	while (h->len) {
		i = popfromheap(h);
		if (i == b)
			break;
		v = g->vertices[i];
		v->visited = 1;
		for (j = 0; j < v->elength; j++) 
		{
			edge1 *e = v->edges[j];
			vertex1 *u = g->vertices[e->vertex];
			if (!u->visited && v->distance + e->weight <= u->distance) 
			{
				u->prev = i;
				u->distance = v->distance + e->weight;
				pushinheap(h, e->vertex, u->distance);
			}
		}
	}
}

void printpath (graph1 *g, int i) 
{
	int n, j;
	vertex1 *v, *u;
	i = i - 'a';
	v = g->vertices[i];
	if (v->distance == INT_MAX) 
	{
		printf("no path\n");
		return;
	}
	for (n = 1, u = v; u->distance; u = g->vertices[u->prev], n++)
		;
	char *path = (char*)malloc(n);
	path[n - 1] = 'a' + i;
	for (j = 0, u = v; u->distance; u = g->vertices[u->prev], j++)
		path[n - j - 2] = 'a' + u->prev;
	
	printf("Path cost = %d ; Path =  %.*s\n",v->distance, n, path);

}

int main () 
{

	clock_t t;
	t = clock();

	graph1 *g = (graph1*)calloc(1, sizeof (graph1));

	FILE *fp;
	char value[50];
	fp=fopen("dt01.txt","r");
	if(fp==NULL)
	{
		printf("Can't open");
		return 1;  
	}

	char *line;
	char x,y,z;
	line=fgets(value,50,fp);

	
	int j = 0;
	char *p = strtok (line, " ");
	char *array[3];

	while (p != NULL)
	{
		array[j++] = p;
		p = strtok (NULL, " ");
	}

	char v1=*array[2];
	
	int v=atoi(array[0]);

	line=fgets(value,50,fp);

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


		if(v1 == 'U')
		{	
			addedge(g, x, y, z);
			addedge(g, y ,x, z);
		}
		else
		{	
			addedge(g, x, y, z);
		}

		line=fgets(value,50,fp);
	}

	char so,f,r;

	printf("Enter source vertex");
	scanf("%c",&so);

	f='a';
	r=so+1;
	int w=v;

	while(f<so)
	{
		dijkstra(g, so, f);	
		f++;
		w--;
	}

	while(w>0)
	{
		dijkstra(g, so, r);
		r++;
		w--;
	}


	w='a';
	for(int k = 0;k < v; k++)
	{
		printpath(g, w);	
		w=w+1;
	}

	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC;
	printf("\nRun time=%f seconds\n", time_taken);
}