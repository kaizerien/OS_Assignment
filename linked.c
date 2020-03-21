#include "linked.h"
#include "shared.h"
#include "vcb.c"

int linked(void)
{

    printf("Enter name of .csv file to read from\n");
    //scanf("%s", &input);
    //Enter .CSV file to run
    linked_readCSV("SampleCSV.csv");

    //Print everything at end of program
    printallocation();
}

void linkedAllocation(char *data)
{
    int file_name;

    if (!strcmp(data, "add"))
    {
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
        if (atoi(data) != 0 && atoi(data) % 100 == 0)
        {
            file_name = atoi(data);
            //Check for avaliable block
            if (checkfsm() != -1)
            {
                curBlock = checkfsm();
                printf("Adding file %i and found free block %i\n", file_name, curBlock);
            }
            else
            {
                printf("Not enough space on disk");
            }
            filesize = 0;
            filelength = 1;
            //Add file to directory
            updateDirectory(curBlock, file_name, add);
        }
        else
        {
            if (atoi(data) != 0)
            {
                //If filesize reached the limit of the blockSize
                if (filesize == blockSize - 1)
                {
                    //If next block is free //+2 Reserve a space
                    if (checkfree(curBlock + 1) == TRUE)
                    {
                        int previous_curBlock = curBlock; // Store the tempory block
                        char char_curBlock[10];

                        //Ensure next block is free to linked
                        curBlock = checkfsm();

                        sprintf(char_curBlock, "%d", curBlock);
                        nodes[previous_curBlock][blockSize - 1].filename = file_name;
                        strcpy(nodes[previous_curBlock][blockSize - 1].data, char_curBlock);

                        //                        updateDirectory(((previous_curBlock + 1) - filelength), file_name, overwrite);

                        printf("Adding file %i and found free block %i\n", file_name, curBlock);

                        filesize = 0;
                        filelength += 1;
                        savetofile(file_name, data);
                        //Overwrite old directory entry to update length
                        updateDirectory(((curBlock + 1) - filelength), file_name, overwrite);
                    }
                    else
                    {
                        //Incomplete code, supposed to find blocks which can fit
                        printf("Disk not enough space for file!");
                    }
                    //Save data to block
                }
                else
                {
                    savetofile(file_name, data);
                }
                filesize += 1;
            }
        }
        vcbfunc(nodes);
        strcpy(nodes[0][0].data, vcbString);
        break;
    case read:
        if (atoi(data) != 0)
        {
            //Find if file exist
            findreadfile(data);
        }
        vcbfunc(nodes);
        strcpy(nodes[0][0].data, vcbString);
        break;
    case delete:
        if (atoi(data) != 0 && atoi(data) % 100 == 0)
        {
            //Find if file exist
            finddeletefile(atoi(data));
        }
        vcbfunc(nodes);
        strcpy(nodes[0][0].data, vcbString);
        break;
    }
}

void linked_savetofile(int f, char d[])
{
    if (checkfree(curBlock) == TRUE)
    {
        updatefsm(curBlock);
    }
    //Reserve a space to input the pointer
    for (int i = 0; i < blockSize; i++)
    {
        if (checkspace(curBlock, i) == TRUE)
        {
            nodes[curBlock][i].filename = f;
            strcpy(nodes[curBlock][i].data, d);
            printf("%d\t%d\t%d\t%s\n", nodes[curBlock][i].index, nodes[curBlock][i].blockNo, nodes[curBlock][i].filename, nodes[curBlock][i].data);
            return;
        }
    }
}

void linked_readCSV(char input[])
{
    register int count = 0;
    FILE *fp = fopen(input, "r");

    char buffer[1024];
    while (fgets(buffer, 1024, (FILE *)fp))
    {
        count++;
        if (count != 0)
        {
            linked_getData(buffer);
        }
    }

    fclose(fp);
}

//Func to input read data into allocation method
void linked_getData(char buffer[])
{
    char *token = strtok(buffer, ", ");
    char copy[1000][1000];
    int counter = 0;
    while (token)
    {
        strcpy(copy[counter], token);
        //contiguousAllocation(token);
        token = strtok(NULL, ", ");
        counter++;
    }
    for (int i = 0; i < counter; i++)
    {
        linkedAllocation(copy[i]);
    }
}

