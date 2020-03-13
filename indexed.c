#include "indexed.h"

// int indexed(void)
// {

//     printf("Enter name of .csv file to read from\n");
//     //scanf("%s", &input);
//     readCSV("MyCSV.csv");

//     printallocation();
// }

void index_allocation(char *data, char choice)
{
    //char choice;
    int blockNum;
    if (!strcmp(data, "add"))
    {
        blockNum = findNewBlock(); //Once add is read, find a block Num for allocation.
        choice = add;
    }
    else if (!strcmp(data, "read"))
    {
        choice = read;
    }
    else if (!strcmp(data, "delete"))
    {
        choice = delete;
    }
    switch (choice)
    {
    case add:
        //If it is a file name then go inside if
        if (atoi(data) != 0 && atoi(data) % 100 == 0)
        {
            filesize = 0;    // reset counter for file size
            columncount = 0; // reset counter for columncount
            //strcpy(file_name, data);
            file_name = atoi(data);
            //Reserve index block for each fileName
            reserveBlock(file_name);

            //If it is not a file num
        }
        else
        {
            if (atoi(data) != 0)
            {
                filesize += 1; //Counter + 1 each time to count size.
                if (blockSize * blockSize > filesize)
                {

                    if (columncount == blockSize)
                    { //Reset columncount to zero once bigger than blockSize
                        columncount = 0;
                    }

                    if (filesize > blockSize)
                    {
                        // Save to file function will add data to nodes with global-variable blockNum.
                        printf("If file size more than block Size \n");
                        blockNum = findNewBlock();
                        printf("New Block number: %i \n", blockNum);
                        printf("Column count %i \n", columncount);
                        savetofile(file_name, data, blockNum);
                    }
                    else
                    {
                        savetofile(file_name, data, blockNum);
                    }
                    columncount += 1; //Iterate this variable so i can use in savetofile.
                }
                else
                { //If file size more than what index block can store, stopped adding and deleted the file itself.
                    printf("File Too Big!!!! Stopped adding of file.");
                    //Add function to check for file num using node.data();
                    // deletefile();
                    return;
                }
            }
        }
        break;
    case read:
        //printf("=");
        break;
    case delete:
        //if filename
        if (atoi(data) != 0 && atoi(data) % 100 == 0)
        {
            deletefile(atoi(data));
            int success = updateReserved(atoi(data));
            if (success == 0)
            {
                printf("Deletion of File %i and its index block failed \n", file_name);
            }
            else if (success == 1)
            {
                printf("Deletion of File %i and its index block succeeded \n", file_name);
            }
        }
        break;
    }
}

int updateDirectory()
{
}

int updateReserved(int fileName)
{
    int unreservedBlock = 1000; //Just set a number for validation later
    for (int i = 1; i < noOfBlocks; i++)
    {
        if (nodes[i][1].reserved == fileName)
        {                        //Loop through all the block names and if it is the same as the selected block to unreserve.
            unreservedBlock = i; //select the block to unreserve as the found i.

            break;
        }
    }
    if (unreservedBlock != 1000)
    {
        for (int j = 0; j < blockSize; j++)
        {
            if (nodes[unreservedBlock][j].reserved == fileName)
            {                                           //Loop through all the nodes in block and if it is the same as the selected block to unreserve.
                nodes[unreservedBlock][j].reserved = 0; // set block attribute "reserved" to 0.
            }
        }
        updatefsm(unreservedBlock);
        return 1;
    }
    else
    {
        return 0;
    }
}

int findNewBlock()
{
    int blockNum = randomBlock();
    // Check that block is unused from FSM & manually check empty.
    if ((checkFree(blockNum) == TRUE) && (nodes[blockNum][0].reserved == 0) && strcmp("\0", nodes[blockNum][0].data) == 0 && (nodes[blockNum][0].filename == 0))
    {
        return blockNum;
    }
    else
    {
        findNewBlock();
    }
}
void reserveBlock(int fileName)
{
    int blockNum = findNewBlock();
    for (int i = 0; i < noOfBlocks; i++)
    {
        nodes[blockNum][i].reserved = file_name;
        strcpy(nodes[blockNum][i].data, "-1");
    }
    updatefsm(blockNum);
}

void savetofile(int f, char d[], int blockNum)
{
    // printf("Inside Save to File: %i \n", blockNum);
    // printf("Inside savetofile, filename: %i, data %s : \n", f, d);
    if (checkFree(blockNum) == TRUE)
    {
        updatefsm(blockNum);
        updateIndexBlock(f, blockNum);
    }

    nodes[blockNum][columncount].filename = f;
    strcpy(nodes[blockNum][columncount].data, d);
    printf("%i \n", nodes[blockNum][columncount].filename);
    printf("%s \n", nodes[blockNum][columncount].data);
}

void updateIndexBlock(int f, int blockNum)
{
    char temp[3];
    for (int i = 0; i < noOfBlocks; i++)
    {
        for (int j = 0; j < blockSize; j++)
        {
            if (nodes[i][j].reserved == f)
            {
                if (strcmp(nodes[i][j].data, "-1") == 0)
                {
                    strcpy(nodes[i][j].data, itoa(blockNum, temp, 10));
                    return;
                }
            }
        }
    }
}

int checkspace()
{
    int blockNum = rand() % noOfBlocks;
    for (int i = 1; i < noOfBlocks; i++)
    {
        if (strcmp("\0", nodes[blockNum][i].data) == 0 && (nodes[blockNum][i].filename == 0))
        {
            return blockNum;
        }
        else
        {
            checkspace();
        }
    }
}

void deletefile(int f)
{
    int delblock = 0;
    for (int i = 1; i < noOfBlocks; i++)
    {
        if (nodes[i][1].filename == f)
        {                             //Loop through all the block names and if it is the same as the selected block to delete.
            delblock = i;             //select the block to delete as the found i.
            deleteblock(delblock, f); //Parse into deleteblock
        }
    }
}

void deleteblock(int block, int f)
{
    for (int j = 0; j < blockSize; j++)
    {
        if (nodes[block][j].filename == f)
        {                                       //Loop through selected block
            nodes[block][j].filename = 0;       //Set file name as 0
            strcpy(nodes[block][j].data, "\0"); //Set data as reserved
        }
    }
    updatefsm(block);
}

int randomBlock()
{
    int _randomBlock = rand() % noOfBlocks;
    return _randomBlock;
}

void printallocation()
{
    for (int i = 0; i < noOfBlocks; i++)
    {
        for (int j = 0; j < blockSize; j++)
        {
            // printf("Node Index: %i\n", nodes[i][j].index);
            // printf("Block No: %i\n", nodes[i][j].blockNo);
            // printf("Filename: %i\n", nodes[i][j].filename);
            // printf("Data: %s\n", nodes[i][j].data);
            // printf("Reserved: %s\n", nodes[i][j].reserved);

            printf("%d\t%d\t%d\t%s\t%i \n", nodes[i][j].index, nodes[i][j].blockNo, nodes[i][j].filename, nodes[i][j].data, nodes[i][j].reserved);
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
    int counter = 0;
    while (token)
    {
        //printf("%s\n",token);
        //strcpy(CSV_Data[counter], token);
        // printf(CSV_Data[counter]);
        allocation(token);
        token = strtok(NULL, ", ");

        counter++;
    }
}