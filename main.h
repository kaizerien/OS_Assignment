#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <ctype.h>

#define MAX_STRING_SIZE 100
#define MAX_BLOCK 130
#define FALSE 0
#define TRUE !(FALSE)

struct node
{
    int index;
    int blockNo;
    char data[MAX_STRING_SIZE]; 
    int filename;
}nodes[MAX_BLOCK][MAX_BLOCK];

void initialize();
void updatefsm(int block);
int checkfsm();
int checkfree(int block);

char fsm[MAX_BLOCK];
int blockSize;
int noOfBlocks;
int current_file_number;
int superblockSize;


void updatefsm(int block){
    if(fsm[block - 1] == '1'){
        fsm[block - 1] = '0';
    }else{
        fsm[block - 1] = '1';
    }
}

int checkfsm(){
    for(int i  = 0; i < noOfBlocks-1; i++){
        if(fsm[i] == '1'){
            return i + 1;
        }
    }
}

int checkfree(int block){
    if (fsm[block-1] == '1'){
        return TRUE;
    }else{
        return FALSE;
    }
}

#endif