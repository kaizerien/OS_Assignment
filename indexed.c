#include "indexed.h"
#include "readCSV.c"

void indexed(){
    //Node 0 will be our index block
    int j;
    printf("Inside indexed.c \n");
    for (j=1;j<MAX_BLOCK; j++){
        if (strlen(node[j].data) == 0)
        {
            strcpy(node[j].data, "");
        }
    printf("Node Data: %s\n",node[j].data);

    }

    //1. Find out how to get array from readCSV
    //2. Once can read array, extract out file Num and file Data numbers
    //3. Find once find empty node, store file data and tag file variable for node.

    
    // for (j=0; j<50; j++){
    //     printf("CSV array: %s\n", CSV_Data[j]);

    // }


}