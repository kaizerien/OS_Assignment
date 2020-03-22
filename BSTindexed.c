#include "BSTindexed.h"
#include "vcb.c"
#include "shared.h"
#include "binarysearch.c"

int BSTindexed(void)
{

    printf("Enter name of .csv file to read from\n");
    BSTindex_readCSV("SampleCSV.csv");
    BSTindex_printallocation();
}

//Generate randomblock numbers
int BSTindex_randomBlock()
{
    int _randomBlock = rand() % noOfBlocks;
    return _randomBlock;
}

//read file and call index_getData(buffer)
void BSTindex_readCSV(char input[])
{
    register int count = 0;
    FILE *fp = fopen(input, "r");

    char buffer[1024];
    while (fgets(buffer, 1024, (FILE *)fp))
    {
        count++;
        if (count != 0)
        {
            BSTindex_getData(buffer);
        }
    }

    fclose(fp);
}

//Call the index_allocation for each token read.
void BSTindex_getData(char buffer[])
{
    char *token = strtok(buffer, ",");
    while (token)
    {
        BSTindex_allocation(token);
        token = strtok(NULL, ", ");
    }
}

void BSTindex_allocation(char *data)
{
    int blockNum;
    int first;
    if (!strcmp(data, "add"))
    {
        blockNum = BSTindex_findNewBlock(); //Once add is read, find a block Num for allocation.
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

    //======================================================
    switch (choice)
    {
    case add:
        //If it is a file name then go inside if
        if (atoi(data) != 0 && atoi(data) % 100 == 0)
        {
            strcpy(dataString, "");
            filesize = 0;    // reset counter for file size
            columncount = 0; // reset counter for columncount
            filenodes = 0;   // indication for file using how many nodes
            deleted = 0;     //reset deleted counter for each file
            file_name = atoi(data);
        }
        else
        {
            if (atoi(data) != 0)
            {
                filesize += 1;  //Counter + 1 each time to count size.
                filenodes += 1; // +1 for each iteration of file data to count size
                if (blockSize * blockSize + 1 > filesize)
                { // If index block size can contain the file data, proceed
                    if (columncount == blockSize)
                    { //Reset columncount to zero once reach blockSize
                        columncount = 0;
                    }
                    if (filesize > blockSize)
                    { // If file size bigger than block size
                        if (filenodes - 1 == blockSize)
                        { // Once reach end of block, go ahead and find new block
                            blockNum = BSTindex_findNewBlock();
                            printf("Adding file%i and found free B%i \n", file_name, blockNum);
                            //Insert into BST here for next block, same file
                            root = insert(root, file_name, blockNum);
                            filenodes = 1;
                        }
                        BSTindex_savetofile(file_name, data, blockNum); // Save to file function will add data to nodes with global-variable blockNum.
                    }
                    else
                    {
                        BSTindex_savetofile(file_name, data, blockNum); // Save to file function will add data to nodes with global-variable blockNum.
                    }
                    columncount += 1; //Iterate this variable so i can use in index_savetofile.
                }
                else
                { //If file size more than what index block can store, stopped adding and deleted the file itself. (Validation of index table space)
                    printf("File Too Big!!!! Stopped adding of file. \n");
                    BSTindex_deletefile(file_name);
                    root = deleteNode(root, file_name); //====================
                }
            }
        }
        vcbfunc(nodes);
        strcpy(nodes[0][0].data, vcbString);
        break;
    case read:
        timer = 0;
        // printf("Data is %i", atoi(data));
        if (strcmp(data, "read") == 0)
        { //If the data is "read" command, break because don't need do anything yet.
            return;
        }
        if (atoi(data) != 0 && atoi(data) % 100 == 0)
        { //If read is given a file name straight away, it'll go to index_updateDirectory to output the blocks containing the file.

            printf("\nReading file%i: \n", atoi(data));
            
            BSTindex_updateDirectory(0, atoi(data), read);
            return;
        }
        else if (atoi(data) != 0 && (atoi(data) % 100 != 0))
        { // If its a number thats not divisble by 100, like 105, 203
            BSTindex_findreadfile(data);
            return;
        }

        if (atoi(data) == 0 && readDone == 0)
        {
            printf("There's no file entered! \n");
        }
        vcbfunc(nodes);
        strcpy(nodes[0][0].data, vcbString);

    case delete:
        first = 0;
        if (atoi(data) != 0 && atoi(data) % 100 == 0)
        {
            BSTindex_deletefile(atoi(data));
            root = deleteNode(root, file_name); //==================
        }
        else if (atoi(data) % 100 != 0 && first == 0)
        {
            printf("Please enter a correct file name. \n");
        }
        vcbfunc(nodes);
        strcpy(nodes[0][0].data, vcbString);
        break;
    }
}

void BSTindex_updateDirectory(int blockNum, int file_name, int choice)
{
    if (directEntry < superblockSize)
    {
        // printf("Inside update Directory\n");
        int done = 0;
        char temp[10];
        char *readtoken;
        int length = 0;
        for (int i = 0; i < superblockSize; i++)
        {
            if (choice == add && done != 1)
            {
                if (!strcmp(nodes[0][i + 1].data, "\0"))
                {
                    strcpy(nodes[0][i + 1].data, itoa(file_name, temp, 10));
                    strcat(nodes[0][i + 1].data, ", ");
                    strcat(nodes[0][i + 1].data, itoa(blockNum, temp, 10));
                    done = 1; //Indicate done
                    return;
                }
            }
            else if (choice == read)
            { // Read entry from directory structure
                if (strcmp(nodes[0][i].data, "\0"))
                {                                   //If data is not null
                    strcpy(temp, nodes[0][i].data); //Copy into temp
                    readtoken = strtok(temp, ", "); //Split up by comma
                    blockNum = atoi(readtoken);     //Read entry, compare if blockNum is = to entered data
                    if (atoi(readtoken) == file_name)
                    {
                        readtoken = strtok(NULL, " ");
                        blockNum = atoi(readtoken);
                        for (int i = 0; i < blockSize; i++)
                        {
                            timer++;
                            if (strcmp(nodes[blockNum][i].data, "-1"))
                            { //If there is data, print out the index table's .data which is the blockNum for the file.
                                readDone++;
                                printf("To read file%i, Block %s is being accessed  \n", file_name, nodes[blockNum][i].data); //Show which blocks are being accessed to read
                            }
                        }
                        printf("Time: %i\n\n", timer); //Print timer counter for traversal of array
                    }
                    else if (readDone == 0)
                    {
                        printf("File does not exist! index_updateDirectory \n");
                    }
                }
            }
            else if (choice == delete &&done != 1)
            { // Delete entry structure
                //Splitting up data for each line of directory structure
                strcpy(temp, nodes[0][i].data);
                readtoken = strtok(temp, ", ");
                if (atoi(readtoken) == file_name)
                {
                    strcpy(nodes[0][i].data, "\0");
                    BSTindex_deletefile(file_name);
                    break;
                }
            }
        }
    }
    else if (directEntry < superblockSize)
    {
    }
}

void BSTindex_savetofile(int f, char d[], int blockNum)
{
    char temp[3];
    if (checkfree(blockNum) == TRUE)
    {
        strcat(dataString, "\b");
        strcat(dataString, ")");
        strcat(dataString, ",B");
        sprintf(temp, "%i", blockNum);
        strcat(dataString, temp);
        strcat(dataString, "(");

        updatefsm(blockNum);
        //index_updateIndexBlock(f, blockNum);
    }
    nodes[blockNum][columncount].filename = f;
    strcpy(nodes[blockNum][columncount].data, d);
    strcat(dataString, d);
    strcat(dataString, ",");
}

int BSTindex_findNewBlock()
{
    int blockNum = BSTindex_randomBlock();
    // Check that block is unused from FSM & manually check empty.
    if ((checkfree(blockNum) == TRUE) && (nodes[blockNum][0].reserved == 0) && strcmp("\0", nodes[blockNum][0].data) == 0 && (nodes[blockNum][0].filename == 0))
    {
        return blockNum;
    }
    else
    {
        BSTindex_findNewBlock();
    }
}

void BSTindex_deletefile(int f)
{
    int delblock = 0;
    for (int i = 1; i < noOfBlocks; i++)
    {
        if (nodes[i][1].filename == f)
        {                                      //Loop through all the block names and if it is the same as the selected block to delete.
            delblock = i;                      //select the block to delete as the found i.
            BSTindex_deleteblock(delblock, f); //Parse into deleteblock
        }
    }
}

void BSTindex_deleteblock(int block, int f)
{
    for (int j = 0; j < blockSize; j++)
    {
        if (nodes[block][j].filename == f)
        {                                       //Loop through selected block
            nodes[block][j].filename = 0;       //Set file name as 0
            strcpy(nodes[block][j].data, "\0"); //Set data as reserved
        }
    }
    printf("Block %i is freed. \n", block);
    updatefsm(block);
}

void BSTindex_printallocation()
{
    for (int i = 0; i < noOfBlocks; i++)
    {
        if (i == 0)
        {
            for (int k = 0; k < superblockSize; k++)
            {
                printf("%d\t%d\t%d\t%s\t%i \n", nodes[i][k].index, nodes[i][k].blockNo, nodes[i][k].filename, nodes[i][k].data);
            }
        }
        else
        {
            for (int j = 0; j < blockSize; j++)
            {
                printf("%d\t%d\t%d\t%s\t%i \n", nodes[i][j].index, nodes[i][j].blockNo, nodes[i][j].filename, nodes[i][j].data);
            }
        }
    }
    //BST output
    inorder(root);
    //FSM
    printf("%s\n", fsm);
    printf("Indexed Allocation Method Done");
}

void BSTindex_printvcb()
{
    int i = 0;
    char *token = strtok(nodes[0][0].data, ",");
    while (token)
    {
        strcpy(vcbStringPrint[i], token);
        token = strtok(NULL, ",");
        ++i;
    }

    printf("\n---------------------------\n");
    printf("Number of Blocks:%s\n", vcbStringPrint[0]);
    printf("Block Size:%s\n", vcbStringPrint[1]);
    printf("Number of free:%s\n", vcbStringPrint[2]);
    printf("FSM String:%s\n", vcbStringPrint[3]);
}

void BSTindex_findreadfile(char f[])
{ //Loop through all blocks to find the block with filename, then go to index_updateDirectory function and read.
    // printf("inside index_findreadfile");
    int tempI;
    int tempJ;
    int fileValid;
    for (int i = 1; i < noOfBlocks; i++)
    {
        for (int j = 0; j < blockSize; j++)
        {
            // printf("i is %i, j is %i, data is %i, f is %i\n", i,j, atoi(nodes[i][j].data), atoi(f));
            if (atoi(nodes[i][j].data) == atoi(f))
            {            //Compare .data with f, which is for e.g. 106. Then will use it's blockNum for READ function
                timer++; //Timer variable increment for READ
                if (readDone == 0)
                {
                    printf("Reading file%i(%i): \n", nodes[i][j].filename, atoi(f));
                    BSTindex_updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename, read);
                    fileValid = 1;
                    return;
                }
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
        printf("File does not exist! \n");
    }
}