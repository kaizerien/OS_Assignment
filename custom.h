#include "main.h"

void custom_readCSV(char input[]);
void custom_getData(char *buffer);
void custom_directory_struct(int blockNum, int file_name, int state);

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

        nodes[previousBlock][blockSize - 1].filename = file_name;
        sprintf(char_blockNo, "%d", reserved_block);
        strcpy(nodes[previousBlock][blockSize - 1].data, char_blockNo);
    }

    sprintf(char_blockNo, "%d", blockNum);
    nodes[reserved_block][reserved_block_length].filename = file_name;

    strcpy(nodes[reserved_block][reserved_block_length].data, char_blockNo);
    reserved_block_length++;
}