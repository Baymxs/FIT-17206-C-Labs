#define MAX_NODES 5000
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lib.h"

int main() {

    FILE *fin, *fout;
    struct Edge edges[MAX_NODES];

//Open the file for reading and check whether it is empty
    fin = fopen("in.txt", "r");
    if (fin == NULL)
        return 0;

//Open the file for writting and check whether it is empty
    fout = fopen("out.txt", "w");
    if (fout == NULL)
        return 0;

    int N, M, count_of_the_same_weight;

    read_first_values(fin, fout, &N, &M);

    unsigned int **adjacency_matrix = initilizateMatrixWithInput(fin, fout, N, M, edges, &count_of_the_same_weight);

    int *used_vertex = (int *) malloc((N * sizeof(int)));
    used_vertex = initilizateArrayOfUsedVertex(N, used_vertex);

    dfs(0, N, (int **) adjacency_matrix, used_vertex);

    checkForConnectivityOfGraph(fout, N, used_vertex);

    int *used_vertex_ = initilizateArrayOfUsedVertex1(N, used_vertex);

    PrimAlgorithm(fout, N, (int **) adjacency_matrix, used_vertex_, edges);

    free(adjacency_matrix);
    free(used_vertex);

    return 0;
}