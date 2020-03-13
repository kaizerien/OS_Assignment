#include "main.h"

unsigned char input[MAX_STRING_SIZE];
//char file_name[MAX_STRING];
int file_name;
int filesize;
int columncount;
enum allstate{add = 1, read = 2, delete = 3}state;

void index_allocation(char *token, char choice);
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