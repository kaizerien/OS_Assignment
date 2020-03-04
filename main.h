#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <ctype.h>

#define NO_OF_STRING 50
#define MAX_STRING_SIZE 100
#define MAX_BLOCK 130
#define NO_OF_VCB 50
#define FALSE 0
#define TRUE !(FALSE)

struct node
{
    int index;
    int blockNo;
    char data[MAX_STRING_SIZE]; 
    int filename;
} node[MAX_BLOCK];

struct block
{
    int blockNo;
    struct node* entryNode;
};

int blockSize;
int noOfBlocks;
int current_file_number;
#endif