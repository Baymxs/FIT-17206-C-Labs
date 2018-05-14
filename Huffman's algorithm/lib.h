#ifndef LIB_H
#define LIB_H
#define NUMBER 256

//Character representation
struct Symbol{
    unsigned char ch; //ASCII-code
    double frequency; //Frequency of symbol int he text (0.0 .. 1.0)
    unsigned char symbol_code[NUMBER]; //Prefix code of symbol as a string of '0' and '1'
    int case_of_new_point; // 0 - old point, 1 - we created new point
    struct Symbol *left; //The left child in the tree
    struct Symbol *right; //The right child in the tree
};

void initilizateFrequencyArray(int frequency_of_symbols[]) {
    for(int i = 0; i < NUMBER; i++) {
        frequency_of_symbols[i] = 0;
    }
}

void readData(FILE* fin, FILE* fout, int *all_number, int *unique_number, int frequency_of_symbols[]) {
    unsigned int byte, tmp_return = 1;
    fseek(fin, 3, SEEK_SET);
    while(tmp_return) {
        //Reading one byte from a file
        byte = fgetc(fin);

        //Check for end of file
        if (byte == EOF)
            tmp_return = 0;
        else {
            //Fill frequency array
            if(frequency_of_symbols[byte] == 0)
                *unique_number = *unique_number + 1;
            *all_number = *all_number + 1;
            frequency_of_symbols[byte]++;
        }
    }
    //if we have only one symbol, we need write to file the same same symbol to not waste file memory
    if(*all_number == 1) {
        fseek(fin, 3, SEEK_SET);
        byte = fgetc(fin);
        fprintf(fout, "%c", byte);
        exit(0);
    }
}

void initilizateSymbolArray(int unique_number, int *frequency_of_symbols, struct Symbol *symbol_array) {
    for(int i = 0, j = 0; i < NUMBER; i++) {
        if(frequency_of_symbols[i] != 0) {
            symbol_array[j].case_of_new_point = 0;
            symbol_array[j].ch = i;
            symbol_array[j].frequency = (float)frequency_of_symbols[i]/(float)unique_number;
            symbol_array[j].left = NULL;
            symbol_array[j].right = NULL;
            j++;
        }
    }
}

void Sort(struct Symbol *symbol_array, int unique_number) {
    struct Symbol tmp;
    for(int i = 0; i < unique_number; i++) {
        for(int j = 0; j < unique_number - i - 1; j++) {
            if(symbol_array[j].frequency < symbol_array[j + 1].frequency) {
                tmp = symbol_array[j];
                symbol_array[j] = symbol_array[j + 1];
                symbol_array[j + 1] = tmp;
            }
        }
    }
}

struct Symbol *buildTree(struct Symbol *smb_array[], int unique_number) {

    //Сreate a temporary node
    struct Symbol *tmp = (struct Symbol*)malloc(sizeof(struct Symbol));

    //Put in the field of frequency the sum of frequencies
    //of the last and penultimate elements of the massif of structures registers
    tmp->frequency = smb_array[unique_number - 2]->frequency + smb_array[unique_number - 1]->frequency;

    //Link the created node with the last two nodes
    if(smb_array[unique_number - 1]->case_of_new_point) {
        tmp->right = smb_array[unique_number - 1];
        tmp->left = smb_array[unique_number - 2];
    } else if(!smb_array[unique_number - 1]->case_of_new_point) {
        tmp->left = smb_array[unique_number - 1];
        tmp->right = smb_array[unique_number - 2];
    }

    //We add information, that it's a new point
    tmp->case_of_new_point = 1;

    tmp->symbol_code[0] = 0;

    //The frequency is 1.0 and we got the root
    if(unique_number == 2) {
        return tmp;
    }

    //Fill new node into right place in smb_array
    for(int i = 0; i < unique_number; i++) {
        if (tmp->frequency > smb_array[i]->frequency) {
            for(int j = unique_number - 1; j > i; j--)
                smb_array[j] = smb_array[j - 1];
            smb_array[i] = tmp;
            break;
        }
    }

    return buildTree(smb_array, unique_number - 1);
}

void makeCompressionCodes(FILE* fout, struct Symbol *root, unsigned char *buff, int *tree_used_bits, int *count_tree_smb) {
    //if root is just a new node, we fill bit '1' into right place of byte with shift
    if(root->case_of_new_point) {
        *buff = *buff | 1 << (7 - *tree_used_bits);
        *tree_used_bits = *tree_used_bits + 1;
        //if byte if full, print this byte into out.txt
        if(*tree_used_bits >= 8) {
            fprintf(fout, "%c", *buff);
            *count_tree_smb+=1;
            *tree_used_bits = 0;
            *buff = 0;
        }
    }
    //if we have a node with symbol, we fill bit '0' into right place of byte with shift and then
    //write symbol bits into byte also with shift
    if(!root->case_of_new_point) {
        *tree_used_bits = *tree_used_bits + 1;
        //if byte if full, print this byte into out.txt
        if(*tree_used_bits >= 8) {
            fprintf(fout, "%c", *buff);
            *count_tree_smb+=1;
            *tree_used_bits = 0;
            *buff = 0;
        }
        for(int i = 0; i < 8; i++) {
            *buff = *buff | ((root->ch >> (7 - i)) << (7 - *tree_used_bits));
            *tree_used_bits = *tree_used_bits + 1;
            //if byte if full, print this byte into out.txt
            if(*tree_used_bits >= 8) {
                fprintf(fout, "%c", *buff);
                printf("%d ", *buff);
                *count_tree_smb+=1;
                *tree_used_bits = 0;
                *buff = 0;
            }
        }
    }

    //Here we create symbols codes
    if(root->left) {
        strcpy(root->left->symbol_code,root->symbol_code);
        strcat(root->left->symbol_code,"0");
        makeCompressionCodes(fout, root->left, buff, tree_used_bits, count_tree_smb);
    }
    if(root->right) {
        strcpy(root->right->symbol_code,root->symbol_code);
        strcat(root->right->symbol_code,"1");
        makeCompressionCodes(fout, root->right, buff, tree_used_bits, count_tree_smb);
    }
}

void compressor(FILE *fin, FILE *fout, struct Symbol *smb_array_2, int unique_number, int *compressing_bits) {
    int tmp_return = 1, byte;
    unsigned char buff = 0, tmp_byte = 0;

    fseek(fin, 3, SEEK_SET);
    fprintf(fout, "%c", ' ');

    while(tmp_return) {
        int i = 0;

        //Reading one byte from a file
        byte = fgetc(fin);

        //Check for end of file
        if(byte == EOF) {
            tmp_return = 0;
            if(*compressing_bits != 0) {
                fprintf(fout, "%c", buff);
            }
        } else {
            for(i = 0; smb_array_2[i].ch != byte;) {
                i++;
            }

            int length = strlen(smb_array_2[i].symbol_code);

            for(int j = 0; j < length; j++) {
                if(smb_array_2[i].symbol_code[j] == '1') {
                    tmp_byte = 1 << (7 - *compressing_bits);
                } else if(smb_array_2[i].symbol_code[j] == '0') {
                    tmp_byte = 0 << (7 - *compressing_bits);
                }
                buff = buff | tmp_byte;
                *compressing_bits+=1;
                if(*compressing_bits == 8) {
                    fprintf(fout, "%c", buff);
                    *compressing_bits = 0;
                    buff = 0;
                }
            }
        }
    }
}

void compressionBlock(FILE *fin, FILE *fout) {
    int all_number = 0, unique_number = 0, tree_used_bits = 0, count_tree_smb = 0, compressing_bits = 0, frequency_of_symbols[256];
    unsigned char buff = 0;

    initilizateFrequencyArray(frequency_of_symbols);

    readData(fin, fout, &all_number, &unique_number, frequency_of_symbols);

    struct Symbol **smb_array_1 = (struct Symbol**)malloc((unique_number) * sizeof(struct Symbol*));
    struct Symbol *smb_array_2 = (struct Symbol*)malloc((unique_number) * sizeof(struct Symbol));

    for(int i = 0; i < unique_number; i++) {
        smb_array_1[i] = (struct Symbol*)malloc(sizeof(struct Symbol));
    }

    initilizateSymbolArray(all_number, frequency_of_symbols, smb_array_2);

    Sort(smb_array_2, unique_number);

    for(int i = 0; i < unique_number; i++) {
        smb_array_1[i] = &smb_array_2[i];
    }

    struct Symbol *root = buildTree(smb_array_1, unique_number);

    makeCompressionCodes(fout, root, &buff, &tree_used_bits, &count_tree_smb);

    //Print the last tree byte
    if(tree_used_bits != 0) {
        fprintf(fout, "%c", buff);
        count_tree_smb+=1;
    }

    compressor(fin, fout, smb_array_2, unique_number, &compressing_bits);

    //We go to the end of the tree in the file to print the number of bits that were used in the last byte
    fseek(fout, count_tree_smb, SEEK_SET);
    fprintf(fout, "%d", compressing_bits);

    free(smb_array_1);
    free(smb_array_2);
    free(root);
}

struct Symbol *buildDecompressionTree(FILE *fin, int *number_of_tree_smb, int *byte, int *tree_used_bits) {
    unsigned int bit;

    bit = (*byte >> (7 - *tree_used_bits))&1;
    *tree_used_bits = *tree_used_bits + 1;

    //If we looked at all 8 bits of the byte, we read the next byte
    if(*tree_used_bits == 8) {
        *tree_used_bits = 0;
        *byte = fgetc(fin);
        *number_of_tree_smb+=1;
    }

    struct Symbol *tmp = (struct Symbol*)malloc(sizeof(struct Symbol));

    //In this case we read 8 bits, this 8 bits are symbol
    if(bit == 0) {
        tmp->ch = 0;
        for(int i = 0; i < 8; i++) {
            bit = (*byte >> (7 - *tree_used_bits))&1;
            tmp->ch = tmp->ch | (bit << (7 - i));
            *tree_used_bits = *tree_used_bits + 1;

            //If we looked at all 8 bits of the byte, we read the next byte
            if(*tree_used_bits == 8) {
                *byte = fgetc(fin);
                *tree_used_bits = 0;
                *number_of_tree_smb+=1;
            }
        }
        tmp->case_of_new_point = 0;
        tmp->left = NULL;
        tmp->right = NULL;

        return tmp;
    }

    if(bit == 1) {
        tmp->symbol_code[0] = 0;
    }

    tmp->left = buildDecompressionTree(fin, number_of_tree_smb, byte, tree_used_bits);
    tmp->right = buildDecompressionTree(fin, number_of_tree_smb, byte, tree_used_bits);

    return tmp;
}

void makeDecompressionCodes(struct Symbol *root) {
    //Here we create symbols codes
    if(root->left) {
        strcpy(root->left->symbol_code, root->symbol_code);
        strcat(root->left->symbol_code,"0");
        makeDecompressionCodes(root->left);
    }
    if(root->right) {
        strcpy(root->right->symbol_code, root->symbol_code);
        strcat(root->right->symbol_code,"1");
        makeDecompressionCodes(root->right);
    }

}

void decoder(FILE* fin, FILE* fout, int number_of_tree_smb, long long all_file_bytes, struct Symbol *root) {
    int byte, tree_used_bits = 8, compression_bits, count_of_compression_bits = 0, count_of_symbols = 0, last_byte_flag = 0;
    char bit, buff = 0;

    struct Symbol *current = root;

    fscanf(fin, "%d", &compression_bits);

    while (count_of_symbols != all_file_bytes) {
        while ((current->left != NULL && current->right != NULL) && (count_of_symbols != all_file_bytes)) {
            if (tree_used_bits == 8) {
                byte = fgetc(fin);
                count_of_symbols++;
                if(byte == -1) {
                    byte = 26;
                    fseek(fin, (3 + number_of_tree_smb) + 1 + (count_of_symbols + 1), SEEK_SET);
                }
                if (count_of_symbols == all_file_bytes - 1)
                    last_byte_flag = 1;
                tree_used_bits = 0;
            }

            bit = (byte >> (7 - tree_used_bits)) & 1;
            tree_used_bits++;

            if(last_byte_flag && count_of_compression_bits != (compression_bits + 1)){
                count_of_compression_bits++;
            }

            if((count_of_compression_bits != (compression_bits + 1)) || !last_byte_flag) {
                if (bit == 0)
                    current = current->left;
                if (bit == 1)
                    current = current->right;
            }
        }

        if ((count_of_compression_bits != (compression_bits + 1)) || !last_byte_flag) {
            fprintf(fout, "%c", current->ch);
        }
        current = root;
    }
}

void decompressionBlock(FILE *fin, FILE*fout) {
    int number_of_tree_smb = 1, byte = 0, tree_used_bits = 0;
    long long all_file_bytes;

    //Count the number of characters in the file
    fseek(fin, 0, SEEK_END);
    all_file_bytes = ftell(fin);

    //We use fseek to start reading the tree from the file
    fseek(fin, 3, SEEK_SET);

    byte = fgetc(fin);

    struct Symbol *root = buildDecompressionTree(fin, &number_of_tree_smb, &byte, &tree_used_bits);

    makeDecompressionCodes(root);

    decoder(fin, fout, number_of_tree_smb, all_file_bytes - number_of_tree_smb - 3, root);

}
#endif