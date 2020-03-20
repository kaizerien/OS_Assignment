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
    int reserved;
}nodes[MAX_BLOCK][MAX_BLOCK];

void initialize();
void updatefsm(int block);
int checkfsm();
int checkfree(int block);

char fsm[MAX_BLOCK];
int blockSize;
int noOfBlocks;
int current_file_number;
int extraSpace;
int superblockSize;

char vcbStringPrint[MAX_BLOCK][MAX_BLOCK];
char vcbString[MAX_BLOCK];
void updatefsm(int block);
void vcbfunc(struct node nodes[MAX_BLOCK][MAX_BLOCK]);


//Func to update fsm after block is used(0) or freed(1)
void updatefsm(int block){
    if(fsm[block - 1] == '1'){
        fsm[block - 1] = '0';
    }else{
        fsm[block - 1] = '1';
    }
}

//Func to return free block
int checkfsm(){
    int space = FALSE;
    int block = 0;
    for(int i  = 0; i < noOfBlocks-1; i++){
        if(fsm[i] == '1'){
            block =  i + 1; //Plus 1 bcause fsm does not include superblock
            space = TRUE;
            break;
        }
    }
    if(space == TRUE){
        return block; //Return free block number if found free block
    }else{
        return -1; //Return -1 if no free block found
    }
}

//Func to check if certain block is free
int checkfree(int block){
    if (fsm[block-1] == '1'){
        return TRUE;
    }else{
        return FALSE;
    }
}


#endif