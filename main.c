/*----------------------------------------------------------------------------------------------
Project: CSC1007 - Operating Systems Assignment
Program name: OS File System
Authors: Xie Yuan Shan, Kwang Qi Rong, Hui Jie, Sharon, June, Hui Ting
Group: ???
Date Created: 16 January 2020
Description: 
-----------------------------------------------------------------------------------------------*/

#include "main.h"
#include "contiguous.c"
//include "indexed.c"
//#include "linked.c"

int main(void)
{
    register int i = 0;
    register int j = 0;
    unsigned char input[20];
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
        //Convert the input to become a int
        blockSize = atoi(input);
    } while (blockSize < 0 || blockSize > 130);

    noOfBlocks = MAX_BLOCK / blockSize;
    //If the max block is not divisible by the blocksize, the no of block will be -1
    noOfBlocks = (MAX_BLOCK % blockSize) ? noOfBlocks - 1 : noOfBlocks;

    initialize();

    //File system choice
    do
    {
        printf("\nEnter choice for file system: \n1: Contiguous Allocation\n2: Linked Allocation\n3: Indexed Allocation\n4: Custom Allocation\n");
        scanf("%s", &input);
        choice_allocation = atoi(input);
        if (choice_allocation <= 0 || choice_allocation > 4)
        {
            printf("Invalid choice, please enter between 1 - 4");
        }
        // if (choice == 1)
        // {
        //     //Space for Contiguous
        //     contiguous();
        // }
        // else if (choice == 2)
        // {

        //     //Space for Linked
        // }
        // else if (choice == 3)
        // {
        //     //  indexed();
        // }
        // else if (choice == 4)
        // {
        //     //Space for custom
        // }
        // else
        //     printf("Invalid choice, please enter between 1 - 4");

    } while (choice_allocation <= 0 || choice_allocation > 4);

    //Require validation
    printf("Enter name of .csv file to read from\n");
    //scanf("%s", &input);
    readCSV("SampleCSV.csv");
    //printallocation();

    system("pause");
    return EXIT_SUCCESS;
}

void initialize()
{
    int index = 0;
    for (int i = 0; i < noOfBlocks; i++)
    {
        if (i != 0)
        {
            fsm[i - 1] = '0';
        }
        for (int j = 0; j < blockSize; j++)
        {
            nodes[i][j].blockNo = i;
            strcpy(nodes[i][j].data, "\0");
            nodes[i][j].filename = 0;
            nodes[i][j].index = index;
            index += 1;
            printf("%d\t%d\t%d\t%s\n", nodes[i][j].index, nodes[i][j].blockNo, nodes[i][j].filename, nodes[i][j].data);
        }
    }
    printf("%s", fsm);
}

void readCSV(char input[])
{
    register int count = 0;
    FILE *fp = fopen(input, "r");

    char buffer[1024];
    while (fgets(buffer, 1024, (FILE *)fp))
    {
        count++;
        if (count != 0)
        {
            getData(buffer);
        }
    }

    fclose(fp);
}

void getData(char buffer[])
{
    char *token = strtok(buffer, ",");
    char choice;
    int counter = 0;
    while (token)
    {
        if (!strcmp(token, "add"))
        {
            choice = add;
        }
        else if (!strcmp(token, "read"))
        {
            choice = read;
        }
        else if (!strcmp(token, "delete"))
        {
            choice = delete;
        }
        else
        {
            switch (choice)
            {
            case 1:
                cont_allocation(token, choice);
                break;
            case 2:
                //link_allocation(token, choice);
                break;
            case 3:
                //index_allocation(token, choice);
                break;
            case 4:
                break;
            }
        }
        //printf("%s\n",token);
        token = strtok(NULL, ", ");
        counter++;
    }
}

// void printallocation()
// {
//     for (int i = 0; i < noOfBlocks; i++)
//     {
//         for (int j = 0; j < blockSize; j++)
//         {
//             if (nodes[i][j].filename != 0)
//             {
//                 printf("Node Index: %i\n", nodes[i][j].index);
//                 printf("Block No: %i\n", nodes[i][j].blockNo);
//                 printf("Filename: %i\n", nodes[i][j].filename);
//                 printf("Data: %s\n", nodes[i][j].data);
//             }
//         }
//     }
//     printf("FSM: %s\n", fsm);
// }

