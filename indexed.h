#include "main.h"

unsigned char input[MAX_STRING_SIZE];
//char file_name[MAX_STRING];
int file_name;
int filesize;
int columncount;
int filenodes;
int deleted=0;
int directEntry =0;
enum allstate{add = 1, read = 2, delete = 3}state;

void readCSV(char input[]);
void getData(char *buffer);
void allocation(char *token);
void printallocation();
// int checkspace(int n, int k);
int checkspace();
void savetofile(int f, char d[], int blockNum);
void reserveBlock(int fileName);
int findNewBlock();
int randomBlock();

int updateReserved(int fileName);
void deletefile(int f);
void deleteblock(int block, int f);
void updateIndexBlock(int f, int blockNum);

void updateDirectory(int blockNum, int file_name, int f);
void findreadfile(char f[]);