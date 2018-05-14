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

    //c - Huffman code, d - Huffman decode
    direction = fgetc(fin);

    if(direction == 'c') {
        compressionBlock(fin, fout);
    } else if(direction == 'd') {
        decompressionBlock(fin, fout);
    }


}