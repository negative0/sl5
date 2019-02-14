#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define INF -999
#define NIL -998
#define TRUE 1
#define FALSE 0

typedef struct Edge
{
	int start;
	int end;
	int weight;
}Edge;

int getEdges(int nos, Edge **edges){
	Edge *edg = malloc(sizeof(Edge) * nos);

	int scan_v = -1;
	int i,j, k = 0;
	printf("Enter the edges: put -1 if no edge\n");
	for(i=0;i<nos;i++){
		for(j = 0;j<nos;j++){
			printf("\nEnter edge for vertex [%d,%d]\t",i, j);
			
			scanf("%d", &scan_v);
			if(scan_v != -1){
				edg[k].start = i;
				edg[k].end = j;
				edg[k++].weight = scan_v;
			}
		}
	}
	*edges = edg;
	// printf("Here8");
	return k;
}


void findPath(int p[], int src, int cur, int cnst){
	if(cur == src){
		printf("(%d)->", src);
		return;
	}else{
		// printf("\nsrc=%d,p[cur]=%d,cur=%d\n",src, p[cur], cur );
		findPath(p,src,p[cur], cnst);
		// printf("%d -->",temp );
		printf("(%d)",cur );
		if(cnst != cur){
			printf("->");
		}
		// return p[cur];
	}
	
	
}

void printDistances(int d[],int p[], int k, int src){
	int i;
	// printf("Here5");
	printf("\nVertex\t|Dist\t|Parent\t|Path\t|\n___________________________\n");
	for(i=0;i<k;i++){
		printf("%d\t|%d\t|%d\t|", i, d[i], p[i]);
		// printf("\t");
		if(i != src)
		findPath(p, src, i, i);
		printf("\n");
	}
	// printf("Here6");
	printf("___________________________\n");

}


void bellman_ford(Edge edges[], int src, int vertices, int n_edges){
	int i,j, e;
	int d[vertices], p[vertices];
	int update;

	// Initialization
	for(i=0;i<vertices;i++){
		d[i] = INF;
		p[i] = NIL;
	}
	d[src] = 0;
	p[src] = src;

	update = FALSE;
	// printf("Here");
	// Relaxation
	for(i = 0; i < vertices-1; i++){
		update = FALSE;
		for(e = 0;e < n_edges;e++ ){
			Edge cur = edges[e];
			if( 
				(d[cur.end] == INF && d[cur.start] != INF) ||  
				(d[cur.end] != INF && d[cur.start] != INF && d[cur.end] > d[cur.start] + cur.weight)
			){
				d[cur.end] = d[cur.start] + cur.weight;
				p[cur.end] = cur.start;
				update = TRUE;
			}
			// printf("Here2");
		}
		printf("\nFor Vertex: %d\n",i );
		printDistances(d, p, vertices, src);

		if(update == FALSE){
			// printf("Here3");
			break;
		}
	}
	for(e = 0;e < n_edges;e++ ){
		Edge cur = edges[e];
		// printf("Here4");
		if(d[cur.end] > d[cur.start] + cur.weight){
			printf("\n-ve Cycle found exiting!\n");
			break;
		}
	}

	printDistances(d, p, vertices, src);
	// findPath(p,src,3,3); 
}

int main(int argc, char const *argv[])
{
	
	int no_vertices, src, n_edges;
	Edge *edges = NULL;
	src = INF;
	printf("Enter the number of vertices:\n");
	scanf("%d", &no_vertices);

	n_edges = getEdges(no_vertices, &edges);


	while(src == INF || src < 0 || src > no_vertices -1 ){
		printf("Enter the src vertex:\n");
		scanf("%d", &src);
	}
	// printf("Here7, %d", edges == NULL);
	bellman_ford(edges, src, no_vertices, n_edges);

	printf("\nExiting!\n");

	return 0;
}