#include "custom.h"
#include "shared.h"
#include "vcb.c"

int custom(void)
{

    printf("Enter name of .csv file to read from\n");
    //scanf("%s", &input);
    //Enter .CSV file to run
    custom_readCSV("SampleCSV.csv");

    //Print everything at end of program
    printallocation();
}

void customAllocation(char *data)
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
        //Adding Contiguous
        if (atoi(data) != 0 && atoi(data) % 100 == 0)
        {
            file_name = atoi(data);
            //Check for avaliable block
            if (checkfsm() != -1)
            {
                //Reserverd Block For Indexing
                reserved_block = custom_findNewBlock();
                start_block = reserved_block;
                updatefsm(reserved_block);

                curBlock = checkfsm();
                temp_file_name = file_name;
                reserved_block_length = 0;

                custom_directory_struct(curBlock, file_name, add);
                linked_updateDirectory(start_block, file_name, overwrite, 0);
                printf("Adding file %i and found free block %i\n", file_name, curBlock);
            }
            else
            {
                printf("Not enough space on disk");
            }
            filesize = 0;
            filelength = 1;
            //Add file to directory
            linked_updateDirectory(reserved_block, file_name, add, 0);
        }
        else
        {
            if (atoi(data) != 0)
            {
                filesize += 1;
                //If filesize too big for one block
                if (filesize > blockSize)
                {
                    if (checkfree(curBlock + 1) == TRUE)
                    { //Ensure next block is free because contiguous

                        curBlock = checkfsm();

                        custom_directory_struct(curBlock, temp_file_name, add);
                        printf("Adding file %i and found free block %i\n", file_name, curBlock);
                        filesize = 0;
                        filelength += 1;
                        savetofile(file_name, data);
                        //Overwrite old directory entry to update length
                        linked_updateDirectory(start_block, file_name, overwrite, reserved_block);
                    }
                    else
                    {
                        //Incomplete code, supposed to find blocks which can fit
                        printf("Disk not enough space for file!");
                    }
                }
                else
                {
                    //Save data to block
                    savetofile(file_name, data);
                }
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

void custom_readCSV(char input[])
{
    register int count = 0;
    FILE *fp = fopen(input, "r");

    char buffer[1024];
    while (fgets(buffer, 1024, (FILE *)fp))
    {
        count++;
        if (count != 0)
        {
            custom_getData(buffer);
        }
    }

    fclose(fp);
}

//Func to input read data into allocation method
void custom_getData(char buffer[])
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
        customAllocation(copy[i]);
    }
}
