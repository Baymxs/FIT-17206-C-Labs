#ifndef LIB_H
#define LIB_H
int compare(unsigned int distance[], unsigned int overflow[], unsigned int node1, unsigned int node2, unsigned int length)
{
    if (overflow[node1] < overflow[node2])
        return -1;

    if (overflow[node1] > overflow[node2])
        return 1;

    if (distance[node1] + length < distance[node2])
        return -1;

    if (distance[node1] + length > distance[node2])
        return 1;


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
#endif