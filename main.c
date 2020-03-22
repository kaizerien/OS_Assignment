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
#include "indexed.c"
#include "linked.c"
#include "custom.c"

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
        //Convert the input to become a int
        blockSize = atoi(input);
    } while (blockSize < 0 || blockSize > 130);

    noOfBlocks = MAX_BLOCK / blockSize;
    superblockSize = blockSize + (MAX_BLOCK % blockSize); //Add unused/extra nodes to block 0 aka superblock

    initialize();
   // linked();

    //File system choice
    do
    {
        printf("\nEnter choice for file system: \n1: Contiguous Allocation\n2: Linked Allocation\n3: Indexed Allocation\n4: Custom Allocation\n");
        scanf("%s", &input);
        choice = atoi(input);
        if (choice == 1)
        {
            //Enter Contiguous Allocation
            contiguous();
        }
        else if (choice == 2)
        {

            //Space for Linked
            linked();
        }
        else if (choice == 3)
        {
            indexed();
        }
        else if (choice == 4)
        {
            custom();
            //Space for custom
        }
        else
            printf("Invalid choice, please enter between 1 - 4");
    } while (choice <= 0 || choice > 4);

    return EXIT_SUCCESS;
}

//Func to initialze blocks
void initialize(){
    int index = 0;
    for(int i = 0; i< noOfBlocks ; i++){
        //Initialize FSM string to 1
        if(i != 0){
            fsm[i-1] = '1'; //i-1 because FSM do not include superblock
        }
        if(i == 0){
            for(int k = 0; k < superblockSize; k++){ //For block 0 aka superblock
                nodes[i][k].blockNo = i;
                strcpy(nodes[i][k].data, "\0");
                nodes[i][k].filename = 0;
                nodes[i][k].index = index;
                index += 1;
                printf("%d\t%d\t%d\t%s\n", nodes[i][k].index,nodes[i][k].blockNo, nodes[i][k].filename,nodes[i][k].data);
            } 
        }else{
            for(int j = 0; j< blockSize ; j++){ //For rest of the block
                nodes[i][j].blockNo = i;
                strcpy(nodes[i][j].data, "\0");
                nodes[i][j].filename = 0;
                nodes[i][j].index = index;
                index += 1;
                printf("%d\t%d\t%d\t%s\n", nodes[i][j].index,nodes[i][j].blockNo, nodes[i][j].filename,nodes[i][j].data);
            }
        }
        
    }
    printf("%s", fsm);
}