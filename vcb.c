#include "main.h"
#include <stdbool.h>
#include <ctype.h>
void getBlocks();


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
    // vcbString = (char *)malloc(sizeof(maxblockchar) + sizeof(freeblockschar) + sizeof(sizeofBlockchar) + sizeof(freeblocks));
    strcpy(vcbString, maxblockchar);
    strcat(vcbString, ",");
    strcat(vcbString, sizeofBlockchar);
    strcat(vcbString, ",");
    strcat(vcbString, noFreeBlockschar);
    strcat(vcbString, ",");
    strcat(vcbString, fsm);

    int count = strlen(vcbString);

    vcbString[count + 1] = '\0';
}

// int main()
// {
//     printf("Running");
//     vcbfunc(nodes);
//     printf(vcbString);
//     // for(int i=0;i<MAX_BLOCK;++i){
//     //     printf("%d",node[i].index);
//     // }
//     // printf("%s", vcbString);
// }