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
#include "indexed.c"
#include "indexed.h"

int main(void)
{
    register int i = 0;
    register int j = 0;
    unsigned char input[20];
    unsigned int choice = 0;
    unsigned int format = 0;
    int counter = 0, blockIndex = 0;
    void indexed();
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
    noOfBlocks = (float)MAX_BLOCK / blockSize; // If is 6 , 20 block is created

    noOfBlocks = ((noOfBlocks - (int)noOfBlocks) != 0 ) ? noOfBlocks - 1 : noOfBlocks; // Once divided, if it is not a whole number, the noOfBlocks will -1.

    
    printf("Block\t\tIndex\tFile Data\n");

    while (counter < MAX_BLOCK) //Counter 0 to 130
    {
        // for (j = 0; j < blockSize; j++) //0 to input blockSize, e.g. 0 to 5
        // {
        //     node[j].index = counter;
        //     node[j].blockNo = blockIndex; //Assign e.g. Node 0 to 5 with block Index 0 for first iteration, and 6 to 11 with block Index 1 and so on.

        //     //If counter == 130 entries, j = blockSize to stop the for loop, if not then another if statement. 
        //     // If noOfBlocks less than blockIndex e.g. noOfBlocks is 20, blockIndex will continue over 20, will print "unassigned" and Index. If false, will print Block No, and Index.
        //     (counter == MAX_BLOCK) ? j = blockSize : (noOfBlocks < blockIndex) ? printf("Unassigned\t%d\n", node[j].index): printf("%d\t\t%d\n", node[j].blockNo, node[j].index); 
        //     counter++;
        // }
        // blockIndex++; //Keep iterating +1 so can use in for loop to assign.
        node[counter].index = counter;

        if (node[counter].index % blockSize-1 == 0 && node[counter].index != 0 && node[counter].index != 1){
            blockIndex++;
        }
        
        node[counter].blockNo = blockIndex;

        printf("%d\t\t%d\t\t%d\n", node[counter].blockNo, node[counter].index, blockIndex); 
        counter++;

    }
         
    //Read CSV File
    printf("Enter name of .csv file to read from\n");
    scanf("%s", &input);
    readCSV(input);
    
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
            indexed();
        }
        else if (choice == 4)
        {
            //Space for custom
        }
        else
            printf("Invalid choice, please enter between 1 - 4");
    } while (choice <= 0 || choice > 4);


}