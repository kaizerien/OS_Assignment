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
    int free;
}nodes[MAX_BLOCK][MAX_BLOCK];

void initialize();
void updatefsm(int block);
int checkfsm();

char fsm[MAX_BLOCK];
int blockSize;
int noOfBlocks;
int current_file_number;
int extraSpace;


void updatefsm(int block){
    fsm[block - 1] = '1';
}

int checkfsm(){
    for(int i  = 0; i < noOfBlocks-1; i++){
        if(fsm[i] == '0'){
            return i + 1;
        }
    }
}

int checkfree(int block){
    if (fsm[block] == 0){
        return TRUE;
    }else{
        return FALSE;
    }
}

#endif