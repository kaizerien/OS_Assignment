#include "indexed.h"
#include "vcb.c"

int indexed(void)
{

    printf("Enter name of .csv file to read from\n");
    // scanf("%s", &input);
    index_readCSV("Test2.csv");
    index_printallocation();

}

void index_allocation(char *data)
{
    int blockNum;
    int first;
    if (!strcmp(data, "add"))
    {
        blockNum = index_findNewBlock(); //Once add is read, find a block Num for allocation.
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
    switch (choice)     {
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
            //Reserve index block for each fileName
            if(checkfsm()!= -1){
                //Prevent adding from happening if directory structure is full.
                if (directEntry < superblockSize){  //direct entry is the counter for how many rows directory structure is going to have. superblocksize is block 0 size.
                    index_reserveBlock(file_name);
                    printf("Adding file%i and found free B%i \n", file_name, blockNum);
                    indexDiskSpace = TRUE;
                }
                else if(directEntry>superblockSize-1){
                    printf("\nNot enough space in Directory Structure, adding file%i failed. \n", file_name);
                    directEntry--;
                    indexDiskSpace = FALSE;
                    break;
                }
            }
            else{
                printf("No more available blocks!");
                indexDiskSpace = FALSE;
                break;
            }
        
        }

        //If it is not a file num, it is data e.g. 101, 102,103.
        else
        {
            if (atoi(data) != 0 && indexDiskSpace == TRUE)
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
                            blockNum = index_findNewBlock();
                            printf("Adding file%i and found free B%i \n", file_name, blockNum);
                            filenodes = 1;
                        }
                        index_savetofile(file_name, data, blockNum); // Save to file function will add data to nodes with global-variable blockNum.
                    }
                    else
                    {
                        index_savetofile(file_name, data, blockNum); // Save to file function will add data to nodes with global-variable blockNum.
                    }
                    columncount += 1; //Iterate this variable so i can use in index_savetofile.
                }
                else
                { //If file size more than what index block can store, stopped adding and deleted the file itself. (Validation of index table space)
                    printf("File Too Big!!!! Index Block not enough space, reverting! \n");
                    index_deletefile(file_name);
                    if (deleted == 0)
                    { //So won't keep updating the reserved blocks
                        int success = index_updateReserved(file_name);
                        index_updateDirectory(0, file_name, delete);
                        if (success == 0)
                        {
                            printf("Deletion of File %i and its index block failed \n", file_name);
                        }
                        else if (success == 1)
                        {
                            deleted = 1;
                            printf("Deletion of File %i and its index block succeeded \n", file_name);
                            // index_printvcb();
                        }
                    }
                }
            }
        }
        break;
    case read:
        timer = 0;
        // printf("Data is %i", atoi(data));
        if (strcmp(data, "read") == 0)
        { //If the data is "read" command, break because don't need do anything yet.
            readDone = 0;
            return;
        }
        if (atoi(data) != 0 && atoi(data) % 100 == 0 && readDone == 0)
        { //If read is given a file name straight away, it'll go to index_updateDirectory to output the blocks containing the file.

            printf("\nReading file%i: \n", atoi(data));
            index_updateDirectory(0, atoi(data), read);
            readDone = 1;
            return;
        }
        else if (atoi(data) != 0 && (atoi(data) % 100 != 0) && readDone == 0)
        { // If its a number thats not divisble by 100, like 105, 203
            index_findreadfile(data);
            return;
        }

        if (atoi(data) == 0 && readDone == 0)
        {
            printf("There's no file entered! \n");
        }
    

    case delete:
        first = 0;
        //if filename
        if (atoi(data) != 0 && atoi(data) % 100 == 0)
        {
            index_deletefile(atoi(data));
            int success = index_updateReserved(atoi(data));
            index_updateDirectory(0, atoi(data), delete);
            if (success == 0)
            {
                first = 0;
                printf("Deletion of File %i and its index block failed \n", atoi(data));
            }
            else if (success == 1)
            {
                first = 1;
                printf("Deletion of File %i and its index block succeeded \n", atoi(data));
                // index_printvcb();
                vcbfunc(nodes);

            }
        }
        else if (atoi(data) % 100 != 0 && first == 0)
        {
            printf("Please enter a correct file name. \n");
        }
        break;
    }
}

void index_findreadfile(char f[])
{ //Loop through all blocks to find the block with filename, then go to index_updateDirectory function and read.
    // printf("inside index_findreadfile");
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
                    printf("\nReading file%i(%i): \n", nodes[i][j].filename, atoi(f));
                    index_updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename, read);
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
        printf("\nFile content not linked to file! \n");
    }
}

// void checkDirectory(){

// }

void index_updateDirectory(int blockNum, int file_name, int choice)
{
    if (directEntry < superblockSize)
    {
        // printf("Inside update Directory\n");
        int done = 0;
        char temp[10];
        char *readtoken;
        int length = 0;
        for (int i = 1; i < superblockSize; i++)
        {
            if (choice == add && done != 1)
            {
                if (!strcmp(nodes[0][i].data, "\0"))
                { //If found the next empty node in directory structure, fill in the data.
                    strcpy(nodes[0][i].data, itoa(file_name, temp, 10));
                    strcat(nodes[0][i].data, ", ");
                    strcat(nodes[0][i].data, itoa(blockNum, temp, 10));
                    done = 1; //Indicate done
                    // printf("%s\n", nodes[0][i+1].data);
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
                    // printf("%s \n", readtoken);
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
                // printf("%s", readtoken);
                if (atoi(readtoken) == file_name)
                {
                    strcpy(nodes[0][i].data, "\0");
                    index_deletefile(file_name);
                    break;
                }
            }
        }
    }
}

int index_updateReserved(int fileName)
{
    int unreservedBlock = 1000; //Just set a number for validation later
    for (int i = 1; i < noOfBlocks; i++)
    {
        if (nodes[i][0].reserved == fileName)
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
            {                                                 //Loop through all the nodes in block and if it is the same as the selected block to unreserve.
                nodes[unreservedBlock][j].reserved = 0;       //Set block attribute "reserved" to 0.
                strcpy(nodes[unreservedBlock][j].data, "\0"); //Set data as null
            }
        }
        updatefsm(unreservedBlock);
        directEntry--;
        return 1;
    }
    else
    {
        return 0;
    }
}

int index_findNewBlock()
{
    int blockNum = index_randomBlock();
    // Check that block is unused from FSM & manually check empty.
    if ((checkfree(blockNum) == TRUE) && (nodes[blockNum][0].reserved == 0) && strcmp("\0", nodes[blockNum][0].data) == 0 && (nodes[blockNum][0].filename == 0))
    {
        return blockNum;
    }
    else
    {
        index_findNewBlock();
    }
}
void index_reserveBlock(int fileName)
{
    int blockNum = index_findNewBlock();
    for (int i = 0; i < noOfBlocks; i++)
    {
        nodes[blockNum][i].reserved = fileName;
        strcpy(nodes[blockNum][i].data, "-1");
    }
    printf("\nBlock %i reserved as index block for File %i \n", blockNum, fileName);
    updatefsm(blockNum);
    index_updateDirectory(blockNum, file_name, 1);
    directEntry++;
}

void index_savetofile(int f, char d[], int blockNum)
{
    char temp[3];
    // printf("Inside Save to File: %i \n", blockNum);
    // printf("Inside index_savetofile, filename: %i, data %s : \n", f, d);
    if (checkfree(blockNum) == TRUE)
    {
        strcat(dataString, "\b");
        strcat(dataString, ")");
        strcat(dataString, ",B");
        sprintf(temp, "%i", blockNum);
        strcat(dataString, temp);
        strcat(dataString, "(");

        updatefsm(blockNum);
        index_updateIndexBlock(f, blockNum);
    }
    nodes[blockNum][columncount].filename = f;
    strcpy(nodes[blockNum][columncount].data, d);
    strcat(dataString, d);
    strcat(dataString, ",");
    vcbfunc(nodes);
    // printf("%i \n", nodes[blockNum][columncount].filename);
    // printf("%s \n", nodes[blockNum][columncount].data);
}

void index_updateIndexBlock(int f, int blockNum)
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

// int checkspace(){
//     int blockNum = rand()% noOfBlocks;
//     for (int i = 1; i<noOfBlocks; i++){
//         if (strcmp("\0", nodes[blockNum][i].data) == 0 && (nodes[blockNum][i].filename == 0)){
//             return blockNum;
//         }
//         else{
//             checkspace();
//         }
//     }
// }

void index_deletefile(int f)
{
    int delblock = 0;
    for (int i = 1; i < noOfBlocks; i++)
    {
        if (nodes[i][1].filename == f)
        {                                   //Loop through all the block names and if it is the same as the selected block to delete.
            delblock = i;                   //select the block to delete as the found i.
            index_deleteblock(delblock, f); //Parse into deleteblock
        }
    }
}

void index_deleteblock(int block, int f)
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
    // index_updateDirectory(block, file_name, 1);
}

int index_randomBlock()
{
    int _randomBlock = rand() % noOfBlocks;
    return _randomBlock;
}

void index_printallocation()
{   
    printvcb();
    printf("Index\tBlock No.\tFile No.\tData\t \n");

    for (int j=0; j<superblockSize; j++){
        printf("%d\t%d\t\t%d\t\t%s \n", nodes[0][j].index, nodes[0][j].blockNo, nodes[0][j].filename, nodes[0][j].data);
    }

    printf("Index\tBlock No.\tFile No.\tData\tReserved For \n");

    for (int i = 1; i < noOfBlocks; i++)
    {
        if (i == 0)
        {
            for (int k = 0; k < superblockSize; k++)
            {
                printf("%d\t%d\t\t%d\t%s\t\t%i \n", nodes[i][k].index, nodes[i][k].blockNo, nodes[i][k].filename, nodes[i][k].data, nodes[i][k].reserved);
            }
        }
        else
        {
            for (int j = 0; j < blockSize; j++)
            {

                printf("%d\t%d\t\t%d\t\t%s\t\t%i \n", nodes[i][j].index, nodes[i][j].blockNo, nodes[i][j].filename, nodes[i][j].data, nodes[i][j].reserved);
            }
        }
    }
    printf("%s\n", fsm);
    printf("Indexed Allocation Method Done");
}

void index_readCSV(char input[])
{
    register int count = 0;
    FILE *fp = fopen(input, "r");

    char buffer[1024];
    while (fgets(buffer, 1024, (FILE *)fp))
    {
        count++;
        if (count != 0)
        {
            index_getData(buffer);
        }
    }

    fclose(fp);
}

//This program runs such that the commands need to be in a structured instructions way such like the SampleCSV.csv
//An instruction line would need to be for e.g. "Read, 107" The first segment and the last segment have to there for the program to work.
//If it is just "Read, ", nothing would happen.
void index_getData(char buffer[])
{
    char *token = strtok(buffer, ",");
    while (token)
    {
        index_allocation(token);
        token = strtok(NULL, ", ");
    }
}
// void index_printvcb()
// {
//     int i = 0;
//     char *token = strtok(nodes[0][0].data, ",");
//     while (token)
//     {
//         strcpy(vcbStringPrint[i], token);
//         token = strtok(NULL, ",");
//         ++i;
//     }

//     printf("\n---------------------------\n");
//     printf("Number of Blocks:%s\n", vcbStringPrint[0]);
//     printf("Block Size:%s\n", vcbStringPrint[1]);
//     printf("Number of free:%s\n", vcbStringPrint[2]);
//     printf("FSM String:%s\n", vcbStringPrint[3]);
//     printf("\n---------------------------\n");

// }