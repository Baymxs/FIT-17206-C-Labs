#ifndef LIB_H
#define LIB_H

//Depth search in the graph
void dfs(int vertex, int number_of_vertex, int **adjacency_matrix, int *used_vertex){
    used_vertex[vertex] = 1;
    int i;
    for(i = 1; i <= number_of_vertex; i++) {
        if(adjacency_matrix[vertex][i] && !used_vertex[i]){
            dfs(i, number_of_vertex, adjacency_matrix, used_vertex);
        }
    }
}

//We read the number of vertices and edges, and perform their verification
int read_first_values(FILE *fIn, FILE *fout, int *N, int *M)
{
    int right_input = 0;

    right_input = fscanf(fIn, "%d%d", N, M);
    if ( right_input < 2)
    {
        fprintf(fout, "%s", "bad number of lines");
        exit(0);
    }

    if (!(0 <= *N && *N <= MAX_NODES))
    {
        fprintf(fout, "%s", "bad number of vertices");
        exit(0);
    }

    if (!(0 <= *M && *M <= (*N) * (*N + 1) / 2))
    {
        fprintf(fout, "%s", "bad number of edges");
        exit(0);
    }

    return 0;
}
#endif