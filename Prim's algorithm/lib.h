#ifndef LIB_H
#define LIB_H

struct Edge{
    int index1;
    int index2;
    int weight;
};

//We read the number of vertices and edges, and perform their verification

//We read the number of vertices and edges, and perform their verification
int read_first_values(FILE *fIn, FILE *fout, int *N, int *M)
{
    int right_input = 0;

    right_input = fscanf(fIn, "%d%d", N, M);

    if(*N == 0 && *M == 0) {
        fprintf(fout, "%s", "no spanning tree");
        exit(0);
    } else if(*N == 1 && *M == 0) {
        fprintf(fout, "%s", "");
        exit(0);
    }

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

//This function reads the input data and processes the errors associated with them
int inputDataWithCheck(FILE *fin, FILE *fout, int low_limit, int upper_limit, char *error_message)
{
    int temp = 0;

    fscanf(fin, "%d", &temp);

    if ((temp < low_limit) || (upper_limit < temp))
    {
        fprintf(fout,"%s", error_message); //displaying the error message
        exit(0); //immediate completion of the program
    }

    return temp;
}


unsigned int** initilizateMatrixWithInput(FILE *fin, FILE *fout, int number_of_vertex, int number_of_edges, struct Edge *edges, int *count_of_the_same_weight) {

    //We allocate memory for the cost table and fill in the cost table with initial values(INT_MAX)
    unsigned int **adjacency_matrix = (unsigned int **) malloc(((number_of_vertex + 1) * sizeof(unsigned int *)));
    int i;
    int j;
    for (i = 0; i < number_of_vertex; i++) {
        adjacency_matrix[i] = (unsigned int *) malloc(((number_of_vertex + 1) * sizeof(unsigned int)));

        for (j = 0; j < number_of_vertex; j++)
            adjacency_matrix[i][j] = INT_MAX;
    }

    int index_from, index_to, weight, tmp_weight;

    for (i = 0; i < number_of_edges; i++) {

        //Read the vertices and cost of the transition
        int right_input = fscanf(fin, "%d %d %d", &index_from, &index_to, &weight);

        //We consider the case when the weight is INT_MAX
        if(weight == INT_MAX) {
            weight = INT_MAX - 1;
        }

        //We perform input validation
        if (right_input != 3)
        {
            fprintf(fout, "%s", "bad number of lines");
            exit(0);
        }

        if ((1 > index_from || number_of_vertex < index_from) || (1 > index_to || number_of_vertex < index_to))
        {
            fprintf(fout, "%s", "bad vertex");
            exit(0);
        }

        if ((weight < 0) || (weight > INT_MAX))
        {
            fprintf(fout, "%s", "bad length");
            exit(0);
        }

        //Fill the struct Edge array with values
        edges[i].index1 = index_from;
        edges[i].weight = weight;
        edges[i].index2 = index_to;

        adjacency_matrix[index_from - 1][index_to - 1] = weight;
        adjacency_matrix[index_to - 1][index_from - 1] = weight;
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
void dfs(int vertex, int number_of_vertex, int **adjacency_matrix, int *used_vertex){
    used_vertex[vertex] = 1;
    int i;
    for(i = 0; i < number_of_vertex; i++) {
        if((adjacency_matrix[vertex][i] < INT_MAX) && !used_vertex[i]) {
            dfs(i, number_of_vertex, adjacency_matrix, used_vertex);
        }
    }
}

//We check the graph for connectivity
void checkForConnectivityOfGraph(FILE *fout, int number_of_vertex, int *used_vertex) {
    int i;
    for (i = 0; i < number_of_vertex; i++) {
        if (used_vertex[i] == 0) {
            fprintf(fout, "%s", "no spanning tree");
            exit(0);
        }
    }
}
//Fill the array with initial values (-1)
int* initilizateArrayOfUsedVertex1(int number_of_vertex, int *used_vertex) {
    int i;
    for (i = 0; i < number_of_vertex; i++) {
        used_vertex[i] = -1;
    }
    return used_vertex;
}

//Find the minimum length between the vertex already traversed and not yet traversed
int findMinEdge(int number_of_vertex, int **adjacency_matrix, int *output_vertex_1, int *output_vertex_2, int *used_vertex, struct Edge *edges) {
    int min_edge = INT_MAX;
    int i;
    int j;

    for(i = 0; i < number_of_vertex; i++) {
        if(used_vertex[i] != -1) {
            for (j = 0; j < number_of_vertex; j++) {
                if(used_vertex[j] == -1) {
                    if (adjacency_matrix[i][j] < min_edge) {
                        min_edge = adjacency_matrix[i][j];
                        *output_vertex_1 = i;
                        *output_vertex_2 = j;
                    }
                }
            }
        }
    }
    return min_edge;
}

//Function for output
void printTree(FILE *fout, int number_of_vertex, struct Edge *edges) {
    int i;
    for(i = 0; i < number_of_vertex - 1; i++) {
        fprintf(fout, "%d %d\n", edges[i].index1 + 1, edges[i].index2 + 1);
    }
}

void PrimAlgorithm(FILE *fout, int number_of_vertex, int **adjacency_matrix, int *used_vertex, struct Edge *edges) {
    int output_vertex_1 = 0, output_vertex_2 = 0;
    int i;
    used_vertex[0] = 1;

    for (i = 0; i < number_of_vertex - 1; i++) {

        edges[i].weight = findMinEdge(number_of_vertex, (int **) adjacency_matrix, &output_vertex_1, &output_vertex_2, used_vertex, edges);
        edges[i].index1 = output_vertex_1;
        edges[i].index2 = output_vertex_2;

        used_vertex[output_vertex_2] = 1;
    }

    printTree(fout, number_of_vertex, edges);
}

#endif