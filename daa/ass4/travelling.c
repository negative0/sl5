#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bits/stdc++.h> 

using namespace std;

typedef struct $1{
	int v;
	int **g;
}Graph;

void getGraph(Graph *g){
	int i,j;
	int scan_v = -1;
	g->g = malloc(sizeof(int*) * g->v);
	for(i=0;i<g->v;i++){
		g->g[i] = malloc(sizeof(int*) * g->v);
	}
	for(i=0;i<g->v;i++){
		for(j=0;j<g->v;j++){
			if(i!=j && i<j){
				printf("Enter edge (%d,%d):\t", i,j);
				scanf("%d", &scan_v);
				g->g[i][j] = scan_v;
				g->g[j][i] = scan_v;
			}
		}
	}
	
}

void printGraph(Graph g){
	int i,j;
	printf("\n");
	for(i=0;i<g.v;i++){
		for(j=0;j<g.v;j++){
			printf("%d ",g.g[i][j] );
		}
		printf("\n");
	}
}

int main(int argc, char const *argv[])
{
	Graph g;
	int source;
	printf("Enter number of vertices:\n");
	scanf("%d", &g.v);
	getGraph(&g); 
	printGraph(g);
	return 0;
}