#include "main.h"

// unsigned char input[MAX_STRING_SIZE];
//char file_name[MAX_STRING];
// int file_name;
// int filesize;
// int columncount;
// int filenodes;
// int deleted = 0;
// int directEntry = 1;
// int timer;
// int readDone = 0;
// char dataString[100];
bool indexDiskSpace;

void index_readCSV(char input[]);
void index_getData(char *buffer);
void index_allocation(char *token);
void index_printallocation();
int index_checkspace();
void index_savetofile(int f, char d[], int blockNum);
void index_reserveBlock(int fileName);
int index_findNewBlock();
void index_printvcb();
int index_randomBlock();

int index_updateReserved(int fileName);
void index_deletefile(int f);
void index_deleteblock(int block, int f);
void index_updateIndexBlock(int f, int blockNum);

void index_updateDirectory(int blockNum, int file_name, int f);
void index_findreadfile(char f[]);



