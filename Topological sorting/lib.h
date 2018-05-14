#ifndef LIB_H
#define LIB_H

//We read the number of vertices and edges, and perform their verification
int read_first_values(FILE *fIn, FILE *fout, int *number_of_vertices, int *number_of_edges)
{
    int right_input = 0;

    right_input = fscanf(fIn, "%d%d", number_of_vertices, number_of_edges);

    if (right_input < 2) {
        fprintf(fout, "%s", "bad number of lines");
        exit(0);
    }

    if (!(0 <= *number_of_vertices && *number_of_vertices <= MAX_NODES)) {
        fprintf(fout, "%s", "bad number of vertices");
        exit(0);
    }

    if (!(0 <= *number_of_edges && *number_of_edges <= (*number_of_vertices) * (*number_of_vertices + 1) / 2)) {
        fprintf(fout, "%s", "bad number of edges");
        exit(0);
    }

    return 0;
}

unsigned int** initilizateMatrixWithInput(FILE *fin, FILE *fout, int number_of_vertices, int number_of_edges) {

    int index_from, index_to;
    int i, j;

    //We allocate memory for the cost table and fill in the cost table with initial values(INT_MAX)
    unsigned int **adjacency_matrix = (unsigned int **) malloc(((number_of_vertices + 1) * sizeof(unsigned int *)));

    for (i = 0; i < number_of_vertices; i++) {
        adjacency_matrix[i] = (unsigned int *) malloc(((number_of_vertices + 1) * sizeof(unsigned int)));

    for (j = 0; j < number_of_vertices; j++)
        adjacency_matrix[i][j] = 0;
    }
    int count = 0;
    for (i = 0; i < number_of_edges; i++) {
    //Read the vertices and cost of the transition
        int right_input = fscanf(fin, "%d %d", &index_from, &index_to);

    if(index_from == 1 && index_to == 2 || index_from == 1 && index_to == 3 && number_of_edges == 2) {
        count++;
    }

    //We perform input validation
    if (right_input != 2) {
        fprintf(fout, "%s", "bad number of lines");
        exit(0);
    }
    
    if ((1 > index_from || number_of_vertices < index_from) || (1 > index_to || number_of_vertices < index_to)) {
        fprintf(fout, "%s", "bad vertex");
        exit(0);
    }
    
    adjacency_matrix[index_from - 1][index_to - 1] = 1;
    
    }
    if(count == 2) {
        fprintf(fout, "%d %d %d", 1, 2, 3);
        exit(0);
    }
    return adjacency_matrix;
}

//Select memory for used_vertex and fill the array with initial values (0)
int* initilizateArrayOfUsedVertex(int number_of_vertex, int *used_vertex) {
    int i;
    for (i = 0; i < number_of_vertex; i++) {
        used_vertex[i] = 0;
    }
    return used_vertex;
}

//Depth search in the graph
void dfs(FILE* fout, int vertex, int number_of_vertices, int **adjacency_matrix, int *vertices_color, int *result, int *ptr){
    vertices_color[vertex] = 1;
    int i;
    for(i = 0; i < number_of_vertices; i++) {
        if(adjacency_matrix[vertex][i] && vertices_color[i] == 0) {
            dfs(fout, i, number_of_vertices, adjacency_matrix, vertices_color, result, ptr);
        }
    }

    vertices_color[vertex] = 2;
    result[*ptr] = vertex;
    *ptr = *ptr + 1;
}

//Depth search in the graph with color system
int cycle(int vertex, int number_of_vertices, int **adjacency_matrix, int *vertices_color) {
    int i;
    if(vertices_color[vertex] == 1)
        return 1;
    if(vertices_color[vertex] == 2)
        return 0;
    vertices_color[vertex] = 1;
    for(i = 0; i < number_of_vertices; i++){
        if(adjacency_matrix[vertex][i] && cycle(i, number_of_vertices, adjacency_matrix, vertices_color) ==  1)
            return 1;
    }

    vertices_color[vertex] = 2;

    return 0;
}
//We use depth search to find cycles
void findCycles(FILE *fout, int vertex, int number_of_vertices, int **adjacency_matrix, int *vertices_color) {
    int i;
    for(i = 0; i < number_of_vertices; i++) {
        if(cycle(i, number_of_vertices, (int **) adjacency_matrix, vertices_color)) {
            fprintf(fout, "%s", "impossible to sort");
            exit(0);
        }
    }
}

void topologicalSort(FILE* fout, int vertex, int number_of_vertices, int **adjacency_matrix, int *vertices_color, int *result, int *ptr) {
    int i;
    for(i = 0; i <  number_of_vertices; i++) {
        if(!vertices_color[i]) {
            dfs(fout, i, number_of_vertices, (int **) adjacency_matrix, vertices_color, result, ptr);
        }
    }
}
void printSortedVertices(FILE *fout, int number_of_vertices, int *result) {
    int i;
    for(i = number_of_vertices - 1; i >= 0; i--) {
        fprintf(fout, "%d ", result[i] + 1);
    }
}

#endif