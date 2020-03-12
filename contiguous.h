#include "main.h"

unsigned char input[MAX_STRING_SIZE];
//char file_name[MAX_STRING];
int file_name;

enum allstate{add = 1, read = 2, delete = 3}state;

void readCSV(char input[]);
void getData(char *buffer);
void allocation(char *token);
void printallocation();
int checkspace(int n, int k);
void savetofile(int f, char d[]);