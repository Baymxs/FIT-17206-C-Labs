#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    int direction;
    int size_of_file;

    fseek(fin, 0, SEEK_END);
    size_of_file = ftell(fin);

    //c - Huffman code, d - Huffman decode
    fseek(fin, 0, SEEK_SET);
    direction = fgetc(fin);

    if(direction == 'c' && (size_of_file != 1)) {
        compressionBlock(fin, fout);
    } else if(direction == 'd') {
        decompressionBlock(fin, fout);
    }
}