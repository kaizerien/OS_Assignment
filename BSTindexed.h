#ifndef BST_INDEXED
#define BST_INDEXED
#include "main.h"

// unsigned char input[MAX_STRING_SIZE];
//char file_name[MAX_STRING]; //No need
// int file_name;
// int filesize;
// int columncount;
// int filenodes;
// int deleted = 0;
// int directEntry = 0;
// int timer;
// int readDone = 0;
char dataString[100];
// char choice;

void BSTindex_readCSV(char input[]);
void BSTindex_getData(char *buffer);
void BSTindex_allocation(char *token);
void BSTindex_printallocation();
int BSTindex_checkspace();
void BSTindex_savetofile(int f, char d[], int blockNum);
int BSTindex_findNewBlock();
void BSTindex_printvcb();
int BSTindex_randomBlock();

void BSTindex_deletefile(int f);
void BSTindex_deleteblock(int block, int f);
void BSTindex_updateIndexBlock(int f, int blockNum);

void BSTindex_updateDirectory(int blockNum, int file_name, int f);
void BSTindex_findreadfile(char f[]);
#endif