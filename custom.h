#include "main.h"

void custom_readCSV(char input[]);
void custom_getData(char *buffer);
void custom_directory_struct(int blockNum, int file_name, int state);
void custom_updateDirectory();

int reserved_block;
int reserved_block_length = 0;
int temp_file_name;
int start_block;

int custom_randomBlock()
{
    int _randomBlock = rand() % noOfBlocks;
    return _randomBlock;
}

int custom_findNewBlock()
{
    int blockNum = custom_randomBlock();
    // Check that block is unused from FSM & manually check empty.
    if ((checkfree(blockNum) == TRUE) && (nodes[blockNum][0].reserved == 0) && strcmp("\0", nodes[blockNum][0].data) == 0 && (nodes[blockNum][0].filename == 0))
    {
        return blockNum;
    }
    else
    {
        custom_findNewBlock();
    }
}

void custom_reserveBlock(int fileName, int blockNo)
{
    int blockNum = custom_findNewBlock();
    for (int i = 0; i < noOfBlocks; i++)
    {
        nodes[blockNum][i].reserved = fileName;
        char char_blockNo[10];
        sprintf(char_blockNo, "%d", blockNo);

        strcpy(nodes[blockNum][i].data, char_blockNo);
    }
    printf("\nBlock %i reserved as index block for File %i \n", blockNum, fileName);
    updatefsm(blockNum);
    // index_updateDirectory(blockNum, file_name, 1);
    // directEntry++;
}

void custom_directory_struct(int blockNum, int file_name, int state)
{
    int previousBlock = reserved_block;
    char char_blockNo[10];

    if (reserved_block_length == blockSize - 1)
    {
        reserved_block_length = 0;
        reserved_block = custom_findNewBlock();
        //Initializing
        for (int i = 0; i < blockSize; i++)
        {
            nodes[reserved_block][i].filename = -1;
        }
        updatefsm(reserved_block);

        nodes[previousBlock][blockSize - 1].filename = file_name;
        sprintf(char_blockNo, "%d", reserved_block);
        strcpy(nodes[previousBlock][blockSize - 1].data, char_blockNo);
    }

    sprintf(char_blockNo, "%d", blockNum);
    nodes[reserved_block][reserved_block_length].filename = file_name;

    strcpy(nodes[reserved_block][reserved_block_length].data, char_blockNo);
    reserved_block_length++;
}

void custom_updateDirectory(int blockNum, int file_name)
{
    int done = 0;
    char temp[10];
    char *readtoken;
    int length = 0;
    for (int i = 1; i < blockSize; i++)
    {
        if (strcmp(nodes[0][i].data, "\0"))
        {                                   //If data is not null
            strcpy(temp, nodes[0][i].data); //Copy into temp
            readtoken = strtok(temp, ", "); //Split up by comma
            blockNum = atoi(readtoken);     //Read entry, compare if blockNum is = to entered data

            // printf("%s \n", readtoken);
            if (atoi(readtoken) == file_name)
            {
                readtoken = strtok(NULL, " ");
                blockNum = atoi(readtoken);

                for (int i = 0; i < blockSize; i++)
                {

                    if (strcmp(nodes[blockNum][i].data, "-1") == 1) //If there is data, print out the index table's .data which is the blockNum for the file.
                    {
                        printf("To read file %d, Block %s is being accessed  \n\n", file_name, nodes[blockNum][i].data); //Show which blocks are being accessed to read
                    }
                }
            }
        }
    }   
}

void custom_findreadfile(char f[])
{ //Loop through all blocks to find the block with filename, then go to index_updateDirectory function and read.
    int fileValid;
    for (int i = 1; i < noOfBlocks; i++)
    {
        for (int j = 0; j < blockSize; j++)
        {
            // printf("i is %i, j is %i, data is %i, f is %i\n", i,j, atoi(nodes[i][j].data), atoi(f));
            if (atoi(nodes[i][j].data) == atoi(f))
            { //Compare .data with f, which is for e.g. 106. Then will use it's blockNum for READ function
                printf("the data is %d\n", atoi(nodes[i][j].data));
                 printf("the data is %d\n\n", atoi(f));

                printf("\nReading file%d(%d): \n", nodes[i][j].filename, atoi(f));
                index_updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename, read);
                fileValid = 1;
                return;
            }
            else
            { //If the command is e.g. 307 which is for a file that doesn't exist. It cannot read.
                fileValid = 0;
                continue;
            }
        }
    }
    if (fileValid == 0)
    {
        printf("\nFile content not linked to file! \n");
    }
}
