#ifndef __COMMON_H__
#define __COMMON_H__
#include "main.h"

void printallocation();
int checkspace(int n, int k);
void savetofile(int f, char d[]);
void findreadfile(char f[]);
void linked_findreadfile(char f[]);
void finddeletefile(int f);
void deletecontent(int f);
void updateDirectory(int blockNum, int file_name, int state);
void printreadfile(int blockNo, int filename, int length);
void linked_updateDirectory(int blockNum, int filename, int state, int lastBlock);

unsigned char input[MAX_STRING_SIZE];
int curBlock = 0;
int filesize = 0;
int filelength = 0;
char choice;

int file_name;
int filesize;
int columncount;
int filenodes;
int deleted = 0;
int directEntry = 0;
int timer;
int readDone = 0;

int starting_index = 0;
char dataString[100];

void printallocation()
{
    printf("Final allocation\n");
    printf("Index\tBlock\tFilename\tData\n");
    for (int i = 0; i < noOfBlocks; i++)
    {
        if (i == 0)
        {
            for (int k = 0; k < superblockSize; k++)
            { //Prints block 0 aka superblock
                printf("%d\t%d\t%d\t%s\n", nodes[i][k].index, nodes[i][k].blockNo, nodes[i][k].filename, nodes[i][k].data);
            }
        }
        else
        {
            for (int j = 0; j < blockSize; j++)
            { //Print rest of the block
                printf("%d\t%d\t%d\t%s\n", nodes[i][j].index, nodes[i][j].blockNo, nodes[i][j].filename, nodes[i][j].data);
            }
        }
    }
    printf("FSM: %s\n", fsm);
}

int checkspace(int n, int k)
{
    if (!strcmp(nodes[n][k].data, "\0") && nodes[n][k].filename == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//Code to access directory
void updateDirectory(int blockNum, int filename, int state)
{
    int done = FALSE;
    char temp[10];
    char *readtoken;
    for (int i = 1; i < superblockSize; i++)
    {
        switch (state)
        {
        case add:
            if (done == FALSE)
            {
                //If found the next empty node in directory structure, fill in the data.
                if (!strcmp(nodes[0][i].data, "\0"))
                {
                    strcpy(nodes[0][i].data, itoa(filename, temp, 10));
                    strcat(nodes[0][i].data, ", ");
                    strcat(nodes[0][i].data, itoa(blockNum, temp, 10));
                    strcat(nodes[0][i].data, ", ");
                    strcat(nodes[0][i].data, itoa(filelength, temp, 10));
                    done = TRUE;
                    return;
                }
            }
            break;
        case read:
            //Search directory for file information(name, length)
            if (strcmp(nodes[0][i].data, "\0"))
            {
                strcpy(temp, nodes[0][i].data);
                readtoken = strtok(temp, ", ");
                if (atoi(readtoken) == filename)
                {
                    readtoken = strtok(NULL, ", ");
                    blockNum = atoi(readtoken);
                    readtoken = strtok(NULL, " ");
                    filelength = atoi(readtoken);
                    printreadfile(blockNum, filename, filelength);
                }
            }
            break;
        case delete:
            //Search directory and remove file directory entry
            if (strcmp(nodes[0][i].data, "\0"))
            {
                strcpy(temp, nodes[0][i].data);
                readtoken = strtok(temp, ", ");
                if (atoi(readtoken) == filename)
                {
                    strcpy(nodes[0][i].data, "\0");
                    //Function to delete all entry under this filename
                    deletecontent(filename);
                }
            }
            break;
        case overwrite:
            //For overwrite directory entry when file cannot fit into a single block
            if (strcmp(nodes[0][i].data, "\0"))
            {
                strcpy(temp, nodes[0][i].data);

                readtoken = strtok(temp, ", ");
                if (atoi(readtoken) == filename)
                {
                    strcpy(nodes[0][i].data, "\0");
                    strcpy(nodes[0][i].data, itoa(filename, temp, 10));
                    strcat(nodes[0][i].data, ", ");
                    strcat(nodes[0][i].data, itoa(blockNum, temp, 10));
                    strcat(nodes[0][i].data, ", ");
                    strcat(nodes[0][i].data, itoa(filelength, temp, 10));
                    done = TRUE;
                    return;
                }
            }
            break;
        }
    }
}

void linked_updateDirectory(int blockNum, int filename, int state, int lastBlock_or_file)
{
    int done = FALSE;
    char temp[10];
    char *readtoken;
    for (int i = 1; i < superblockSize; i++)
    {
        switch (state)
        {
        case add:
            if (done == FALSE)
            {
                //If found the next empty node in directory structure, fill in the data.
                if (!strcmp(nodes[0][i].data, "\0"))
                {
                    strcpy(nodes[0][i].data, itoa(filename, temp, 10));
                    strcat(nodes[0][i].data, ", ");
                    strcat(nodes[0][i].data, itoa(starting_index, temp, 10));
                    strcat(nodes[0][i].data, ", ");
                    strcat(nodes[0][i].data, itoa(lastBlock_or_file, temp, 10));
                    done = TRUE;
                    return;
                }
            }
            break;
        case read:
            //Search directory for file information(name, length)
            if (strcmp(nodes[0][i].data, "\0"))
            {
                strcpy(temp, nodes[0][i].data);

                readtoken = strtok(temp, ", ");
                if (atoi(readtoken) == filename)
                {
                    readtoken = strtok(NULL, ", ");
                    blockNum = atoi(readtoken);
                    readtoken = strtok(NULL, " ");
                    filelength = atoi(readtoken);

                    printf("\nFilename of %d,Starting Block is %d ", filename, blockNum);

                    if ((lastBlock_or_file % 100) != 0)
                    {
                        if (strcmp(nodes[blockNum][blockSize - 1].data, "\0"))
                        {
                            int new_blockNum = atoi(nodes[blockNum][blockSize - 1].data);
                            do
                            {
                                printf("-> %d", new_blockNum);
                                new_blockNum = atoi(nodes[new_blockNum][blockSize - 1].data);
                            } while (new_blockNum != 0);
                        }
                    }
                    else
                    {
                        int valid = 0;
                        int new_block_file_content = blockNum;
                        do
                        {

                            for (int i = 0; i < blockSize; i++)
                            {
                                if (i == blockSize - 1)
                                {
                                    if (strcmp(nodes[blockNum][blockSize - 1].data, "\0"))
                                    {
                                        printf("No file content exists!\n");
                                        return;
                                    }
                                    else
                                    {
                                        new_block_file_content = atoi(nodes[new_block_file_content][i].data);
                                    }
                                }
                                else if (atoi(nodes[new_block_file_content][i].data) == lastBlock_or_file)
                                {
                                    printf("\nFile data %d is in Block %d \n", lastBlock_or_file, blockNum);
                                    valid = 1;
                                    return;
                                }
                            }
                        } while (valid == 0);
                    }

                    printf("\n\n");

                    //printreadfile(blockNum, filename, filelength);
                }
            }
            break;
        case delete:
            //Search directory and remove file directory entry
            if (strcmp(nodes[0][i].data, "\0"))
            {
                strcpy(temp, nodes[0][i].data);
                readtoken = strtok(temp, ", ");
                if (atoi(readtoken) == filename)
                {
                    strcpy(nodes[0][i].data, "\0");
                    //Function to delete all entry under this filename
                    deletecontent(filename);
                }
            }
            break;
        case overwrite:
            //For overwrite directory entry when file cannot fit into a single block
            if (strcmp(nodes[0][i].data, "\0"))
            {
                strcpy(temp, nodes[0][i].data);
                readtoken = strtok(temp, ", ");
                if (atoi(readtoken) == filename)
                {
                    strcpy(nodes[0][i].data, "\0");
                    strcpy(nodes[0][i].data, itoa(filename, temp, 10));
                    strcat(nodes[0][i].data, ", ");
                    strcat(nodes[0][i].data, itoa(blockNum, temp, 10));
                    strcat(nodes[0][i].data, ", ");
                    strcat(nodes[0][i].data, itoa(lastBlock_or_file, temp, 10));
                    done = TRUE;
                    return;
                }
            }
            break;
        }
    }
}

//Func to check if file to be read exists
void findreadfile(char f[])
{
    int found = TRUE;
    for (int i = 1; i < noOfBlocks; i++)
    {
        for (int j = 0; j < blockSize; j++)
        {
            if (!strcmp(nodes[i][j].data, f))
            { //If user wants to read by file data
                updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename, read);
                return;
            }
            else if ((atoi(f)) == nodes[i][j].filename)
            { //If user want tor ead by file name
                updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename, read);
                return;
            }
            else
            {
                found = FALSE;
            }
        }
    }
    if (found == FALSE)
    {
        printf("File %s not found on disk\n", f);
    }
}

void linked_findreadfile(char f[])
{
    int found = TRUE;
    for (int i = 1; i < noOfBlocks; i++)
    {
        for (int j = 0; j < blockSize; j++)
        {
            if (atoi(nodes[i][j].data) == atoi(f))
            { //If user wants to read by file data
                linked_updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename, read, atoi(f));
                return;
            }
            else if ((atoi(f)) == nodes[i][j].filename)
            { //If user want tor ead by file name
                linked_updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename, read, i);
                return;
            }
            else
            {
                found = FALSE;
            }
        }
    }
    if (found == FALSE)
    {
        printf("File %s not found on disk\n", f);
    }
}
//Func to output the file user wants to read
void printreadfile(int blockNo, int filename, int length)
{
    printf("Read file %i from block %i\n", filename, blockNo);
    for (int i = 0; i < blockSize; i++)
    {
        if (strcmp(nodes[blockNo][i].data, "\0"))
        {
            printf("%d\t%d\t%d\t%s\n", nodes[blockNo][i].index, nodes[blockNo][i].blockNo, nodes[blockNo][i].filename, nodes[blockNo][i].data);
        }
    }
    //Recursively access other blocks if file uses more then one block
    if (length > 1)
    {
        printreadfile(blockNo + 1, filename, length - 1);
    }
}

//Func to check if file to be deleted exist
void finddeletefile(int f)
{
    int found = TRUE;
    for (int i = 1; i < noOfBlocks; i++)
    {
        for (int j = 0; j < blockSize; j++)
        {
            if (nodes[i][j].filename == f)
            {
                if (checkfree(i) == FALSE)
                {
                    updatefsm(i);
                    printf("Delete file %i and freed Block %i\n", f, i);
                    //Update directory to remove directory entry of file
                    updateDirectory(0, f, delete);
                    return;
                }
                return;
            }
            else
            {
                found = FALSE;
            }
        }
    }
    if (found == FALSE)
    {
        printf("File %i not found on disk\n", f);
    }
}

//Recursive func to delete entry under filename
void deletecontent(int f)
{
    for (int i = 1; i < noOfBlocks; i++)
    {
        for (int j = 0; j < blockSize; j++)
        {
            if (nodes[i][j].filename == f)
            {
                nodes[i][j].filename = 0;
                strcpy(nodes[i][j].data, "\0");
                deletecontent(f);
                return;
            }
        }
    }
}

#endif