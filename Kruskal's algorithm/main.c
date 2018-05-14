#define MAX_NODES 5000
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lib.h"

struct Edge{
    int indexFrom;
    int indexTo;
    int weight;
};

int nodes[MAX_NODES], last_n;

//Comparison function for use in quick sorting
int compareEdges(const void *a,const void *b){
    struct Edge *c = (struct Edge*)a;
    struct Edge *d = (struct Edge*)b;
    return c->weight - d->weight;
}

//Get the "color" of the vertex
int getColor(int n){
    if(nodes[n] < 0)
        return nodes[last_n = n];
    int tmp = getColor(nodes[n]);
    nodes[n] = last_n;
    return tmp;
}

int main() {
    FILE *fin, *fout;

    //Open the file for reading and check whether it is empty
    fin = fopen("in.txt", "r");
    if (fin == NULL)
        return 0;

    //Open the file for writting and check whether it is empty
    fout = fopen("out.txt", "w");
    if (fout == NULL)
        return 0;

    int N, M, index_from, index_to, weight, i, j;

    struct Edge edges[MAX_NODES];

    //Read the number of vertices and edges of the graph
    read_first_values(fin, fout, &N, &M);

    if(N == 0 && M == 0) {
        fprintf(fout, "%s", "no spanning tree");
        return 0;
    } else if(N == 1 || M == 0)
        fprintf(fout, "%s", "");

    //We allocate memory for the adjacency table and fill it with initial values
    int **adjacency_matrix = (int**)malloc(((N+1) * sizeof(int*)));
    for (i = 1; i <= N; i++)
    {
        adjacency_matrix[i] = (int*)malloc(((N+1) * sizeof(int)));

        for (j = 1; j <= N; j++)
            adjacency_matrix[i][j] = 0;
    }

    for (i = 1; i <= M; i++) {
        //Read the vertices and cost of the transition
        int right_input = fscanf(fin, "%d %d %d", &index_from, &index_to, &weight);

        //We perform input validation
        if (right_input != 3)
        {
            fprintf(fout, "%s", "bad number of lines");
            return 0;
        }

        if ((1 > index_from || N < index_from) || (1 > index_to || N < index_to))
        {
            fprintf(fout, "%s", "bad vertex");
            return 0;
        }

        if ((weight < 0) || (weight > INT_MAX))
        {
            fprintf(fout, "%s", "bad length");
            return 0;
        }

        //Filling an array of structures Edge
        edges[i].indexFrom = index_from;
        edges[i].indexTo = index_to;
        edges[i].weight = weight;

        //Filling the table of contiguity
        adjacency_matrix[index_from][index_to] = adjacency_matrix[index_to][index_from] = 1;
    }

    //Create and allocate memory for an array of already traversed vertices
    //This array is used in depth search
    int *used_vertex = (int*)malloc((N * sizeof(int)));
    for(i = 1; i <= N; i++){
        used_vertex[i] = 0;
    }

    //Depth search
    dfs(1, N+1, adjacency_matrix, used_vertex);

    //Checking for connectivity of a graph
    for(i = 1; i <= N; i++){
        if(used_vertex[i] == 0) {
            fprintf(fout, "%s", "no spanning tree");
            return 0;
        }
    }

    //Set the initial "colors" of the vertices
    for (i = 0; i < N; i++)
        nodes[i] = -1 - i;

    //Quick sorting in ascending order of cost
    qsort(edges, M+1, sizeof(struct Edge), compareEdges);

    //We check "colors" at the vertices and print tree
    for(i = 1; i <= M; i++) {
        if(getColor(edges[i].indexTo) != getColor(edges[i].indexFrom)){
            nodes[last_n] = edges[i].indexTo;
            fprintf(fout, "%d %d\n", edges[i].indexFrom, edges[i].indexTo);
        }
    }

    return 0;
}