#ifndef VCB_C
#define VCB_C

#include "main.h"

void vcbfunc(struct node nodes[MAX_BLOCK][MAX_BLOCK])
{

    char maxblockchar[10];
    char sizeofBlockchar[10];
    char noFreeBlockschar[100];
    int noFreeBlocks = 0;
    for (int j = 1; j <= noOfBlocks; ++j)
    {
        if (checkfree(j))
        {
            noFreeBlocks += 1;
        }
    }
    itoa(noOfBlocks, maxblockchar, 10);
    itoa(blockSize, sizeofBlockchar, 10);
    itoa(noFreeBlocks, noFreeBlockschar, 10);
    strcpy(vcbString, maxblockchar);
    strcat(vcbString, ",");
    strcat(vcbString, sizeofBlockchar);
    strcat(vcbString, ",");
    strcat(vcbString, noFreeBlockschar);
    strcat(vcbString, ",");
    strcat(vcbString, fsm);

    int count = strlen(vcbString);

    vcbString[count + 1] = '\0';
    int j = snprintf(nodes[0][0].data, MAX_STRING_SIZE, "%s", vcbString);
}
void index_printvcb()
{
    int i = 0;
    char temp[MAX_STRING_SIZE];
    int j = snprintf(temp, MAX_STRING_SIZE, "%s", nodes[0][0].data);   
    char *token = strtok(nodes[0][0].data, ",");
    while (token)
    {
        strcpy(vcbStringPrint[i], token);
        token = strtok(NULL, ",");
        ++i;
    }
    j = snprintf(nodes[0][0].data, MAX_STRING_SIZE, "%s", temp);
    printf("%s",nodes[0][0].data);

    printf("\n---------------------------\n");
    printf("Number of Blocks:%s\n", vcbStringPrint[0]);
    printf("Block Size:%s\n", vcbStringPrint[1]);
    printf("Number of free:%s\n", vcbStringPrint[2]);
    printf("FSM String:%s\n", vcbStringPrint[3]);
}

#endif