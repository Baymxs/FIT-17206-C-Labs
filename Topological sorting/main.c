#define MAX_NODES 1000
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lib.h"

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

    int number_of_vertices, number_of_edges, i, ptr = 0, cycle_st = -1;

    read_first_values(fin, fout, &number_of_vertices, &number_of_edges);

    unsigned int **adjacency_matrix = initilizateMatrixWithInput(fin, fout, number_of_vertices, number_of_edges);

    int *result = (int *) malloc((number_of_vertices * sizeof(int)));

    int *vertices_color = (int *) malloc((number_of_vertices * sizeof(int)));

    vertices_color = initilizateArrayOfUsedVertex(number_of_vertices, vertices_color);

    findCycles(fout, i, number_of_vertices, (int **) adjacency_matrix, vertices_color);

    vertices_color = initilizateArrayOfUsedVertex(number_of_vertices, vertices_color);

    topologicalSort(fout, i, number_of_vertices, (int **) adjacency_matrix, vertices_color, result, &ptr);

    printSortedVertices(fout, number_of_vertices, result);

    free(adjacency_matrix);
    free(vertices_color);
    free(result);
}