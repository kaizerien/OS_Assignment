#include "main.h"

unsigned char input[MAX_STRING_SIZE];
int curBlock = 0;
int filesize = 0;
int filelength = 0;

void readCSV(char input[]);
void getData(char *buffer);
void contiguousAllocation(char *token);
void printallocation();
int checkspace(int n, int k);
void savetofile(int f, char d[]);
void findreadfile(char f[]);
void finddeletefile(int f);
void deletecontent(int f);
void updateDirectory(int blockNum, int file_name, int state);
void printreadfile(int blockNo, int filename, int length);