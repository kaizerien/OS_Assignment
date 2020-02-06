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

struct node
{
    int index;
    int blockNo;
    int data; //Change because is forever to number
    int filename;
} node[MAX_BLOCK];

struct volume_control_block
{
    int start;
    int end;
} vcb[NO_OF_VCB];

int blockSize;
int noOfBlocks;
int current_file_number;
char CSV_Data[NO_OF_STRING][MAX_STRING_SIZE];
#endif