#define MAX 5000
#define INTM UINT_MAX

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include "lib.h"

int main() {
    FILE *fin, *fout;

//Open the file for reading and check whether it is empty
    fin = fopen("in.txt", "r");
    if(fin == NULL) return 0;

//Open the file for writting and check whether it is empty
    fout = fopen("out.txt", "w");
    if(fout == NULL) return 0;

    int N = inputDataWithCheck(fin, fout, 0, MAX, "bad number of vertices"); //number of vertex

    int S = inputDataWithCheck(fin, fout, 1, N, "bad vertex"); //begin-vertex

    int F = inputDataWithCheck(fin, fout, 1, N, "bad vertex"); //end-vertex

    int M = inputDataWithCheck(fin, fout, 0, N*(N+1)/2, "bad number of edges"); //number of edges

    S--; F--;

    unsigned int **adjacency_matrix = (unsigned int**)malloc((N * sizeof(unsigned int*)));

    for (int i = 0; i < N; i++)
    {
        adjacency_matrix[i] = (unsigned int*)malloc((N * sizeof(unsigned int)));

        for (int j = 0; j < N; j++)
            adjacency_matrix[i][j] = INTM;
    }

    bool already_passed_vertex[MAX]; //already passed vertex

    int parent_vertex[MAX]; //vertex parent

    unsigned int distance[MAX]; //distance from initial vertex

    unsigned int overflow[MAX]; //number of INTM

//Set initial parameters
    for (int i = 0; i < N; i++)
    {
        already_passed_vertex[i] = false;

        if(i == S)
            distance[i] = 0;
        else
            distance[i] = INTM;

        overflow[i] = 0;

        parent_vertex[i] = -1;
    }
//Filling the adjacency_matrix
    for (int i = 0; i < M; i++)
    {
        int v1 = inputDataWithCheck(fin, fout, 1, N, "bad vertex") - 1;
        int v2 = inputDataWithCheck(fin, fout, 1, N, "bad vertex") - 1;
        unsigned int len = inputDataWithCheck(fin, fout, 0, INT_MAX, "bad length\n");

        adjacency_matrix[v1][v2] = len;
        adjacency_matrix[v2][v1] = len;
    }

    for(int i = 0; i < N; i++) {

//We find the vertex, which is closest to the initial vertex
        int temp = -1;
        for (int j = 0; j < N; j++) {
            if (!already_passed_vertex[j] && (temp == -1 || distance[j] < distance[temp]))
                temp = j;
        }

        if (distance[temp] == INTM)
            break;

        already_passed_vertex[temp] = true;

        for (int j = 0; j < N; j++) {
//Skip elements of the matrix whose cost is infinity
            if (adjacency_matrix[temp][j] == INTM || j == parent_vertex[temp])
                continue;

            int length = adjacency_matrix[temp][j];

            if (compare(distance, overflow, temp, j, length) == -1) {
                distance[j] = distance[temp] + length;
                overflow[j] = overflow[temp];

                while (distance[j] > INT_MAX) {
                    overflow[j]++;
                    distance[j] = distance[j] - INT_MAX;
                }
                parent_vertex[j] = temp;
            }
        }
    }
//The first line of output
    for (int i = 0; i < N; ++i)
    {
        if (distance[i] == INTM) fprintf(fout, "oo ");
        else if (overflow[i] == 1) fprintf(fout, "INT_MAX+ ");
        else fprintf(fout, "%d ", distance[i]);
    }

    fprintf(fout, "\n");

    unsigned overflow_count = 0;
    if (overflow[F])
    {
        for (int i = 0; i < N; ++i)
        {
            if (adjacency_matrix[F][i] != INTM && distance[i] + adjacency_matrix[F][i] > INT_MAX)
                overflow_count++;

            if (overflow_count >= 2)
                break;
        }
    }

//The second line of output
    if (S == F)
        fprintf(fout, "%d\n", S + 1);
    else if (parent_vertex[F] == -1)
        fprintf(fout, "no path\n");
    else if (overflow[F] &&
             overflow_count >= 2)
        fprintf(fout, "overflow\n");
    else
    {
//Output the vertices of the shortest path in the reverse order
        for (int path_node = F; path_node != S; path_node = parent_vertex[path_node])
            fprintf(fout, "%d ", path_node + 1);
        fprintf(fout, "%d\n", S + 1);
    }

//Clear memory
    for (int i = 0; i < 1; ++i)
        free(adjacency_matrix[i]);

    free(adjacency_matrix);

    return 0;
}