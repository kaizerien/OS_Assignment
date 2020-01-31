/*----------------------------------------------------------------------------------------------
Project: CSC1007 - Operating Systems Assignment
Program name: OS File System
Authors: Xie Yuan Shan, Kwang Qi Rong, Hui Jie, Sharon, June, Hui Ting
Group: ???
Date Created: 16 January 2020
Description: 
-----------------------------------------------------------------------------------------------*/

#include "main.h"
#include "readCSV.c"

int main(void)
{
    register int i = 0;
    register int j = 0;
    unsigned char input[20];
    unsigned int choice = 0;
    unsigned int format = 0;
    int counter = 0, blockIndex = 0;

    do
    {
        printf("Type Y to format File System or N to exit\n");
        scanf("%s", &input);
        if (strlen(input) == 1)
        {
            if (input[0] == 89 || input[0] == 121) //If user key in Y(89) / y(121)
            {
                //space for format
                format = 1;
            }
            else if (input[0] == 78 || input[0] == 110) ////If user key in N(78) / n(110)
            {
                exit(0);
            }
            else
            {
                printf("Invalid input, please enter between Y or N\n");
            }
        }
        else
        {
            printf("Invalid input, please enter between Y or N\n");
        }

    } while (format != 1);

    do
    {
        printf("\nEnter required block size: ");
        scanf("%s", &input);
        blockSize = atoi(input);
    } while (blockSize < 0 || blockSize > 130);
    noOfBlocks = (float)MAX_BLOCK / blockSize; // If is 5 , 26 block is created

    noOfBlocks = ((noOfBlocks - (int)noOfBlocks) != 0 ) ? noOfBlocks - 1 : noOfBlocks;

    
    printf("Block\t\tIndex\tFile Data\n");

    while (counter < MAX_BLOCK)
    {
        for (j = 0; j < blockSize; j++)
        {
            node[j].blockNo = blockIndex;
            node[j].index = counter;
            (counter == MAX_BLOCK) ? j = blockSize : (noOfBlocks < blockIndex) ? printf("Unassigned\t%d\n", node[j].index): printf("%d\t\t%d\n", node[j].blockNo, node[j].index); 
            counter++;
        }
        blockIndex++;
    }

    //File system choice
    do
    {
        printf("\nEnter choice for file system: \n1: Contiguous Allocation\n2: Linked Allocation\n3: Indexed Allocation\n4: Custom Allocation\n");
        scanf("%s", &input);
        choice = atoi(input);
        if (choice == 1)
        {
            //Space for Contiguous
        }
        else if (choice == 2)
        {
            
            //Space for Linked
        }
        else if (choice == 3)
        {
            //  indexed();
        }
        else if (choice == 4)
        {
            //Space for custom
        }
        else
            printf("Invalid choice, please enter between 1 - 4");
    } while (choice <= 0 || choice > 4);

    //Read CSV File
    printf("Enter name of .csv file to read from\n");
    scanf("%s", &input);
    readCSV(input);
}