#include "indexed.h"

int contiguous(void){

    printf("Enter name of .csv file to read from\n");
    //scanf("%s", &input);
    readCSV("SampleCSV.csv");

    printallocation();
}

void allocation(char *data){
    char choice;
    int blockNum;
    if(!strcmp(data, "add")){
        blockNum = findNewBlock(); //Once add is read, find a block Num for allocation.
        choice = add;
    }else if(!strcmp(data, "read")){
        choice = read;
    }else if(!strcmp(data, "delete")){
        choice = delete;
    }
    switch(choice){
        case add:
            //If it is a file name then go inside if
            if(atoi(data) != 0 && atoi(data)%100 == 0){
                filesize = 0; // reset counter for file size
                columncount =0; // reset counter for columncount
                filenodes = 0; // indication for file using how many nodes
                deleted = 0; //reset deleted counter for each file
                file_name = atoi(data);
                //Reserve index block for each fileName
                reserveBlock(file_name); 
                

            //If it is not a file num, it is data e.g. 101, 102,103.
            }else{
                if(atoi(data)!=0){
                    filesize+=1; //Counter + 1 each time to count size.
                    filenodes+=1; // +1 for each iteration of file data to count size
                    printf("file size : %i\n", filesize );
                    printf("file nodes : %i\n", filenodes );

                    if (blockSize*blockSize+1> filesize){ // If index block size can contain the file data, proceed

                        if (columncount == blockSize){  //Reset columncount to zero once reach blockSize
                            printf("column count resetted \n");
                            columncount = 0;
                        }
                        if (filesize> blockSize){     // If file size bigger than block size 
                            if (filenodes-1 == blockSize){ // Once reach end of block, go ahead and find new block
                                blockNum = findNewBlock();   
                                filenodes = 1;
                            }

                        savetofile(file_name, data, blockNum);  // Save to file function will add data to nodes with global-variable blockNum.
                        }else{
                        savetofile(file_name, data, blockNum); // Save to file function will add data to nodes with global-variable blockNum.
                        }
                        columncount+=1; //Iterate this variable so i can use in savetofile.
                    }
                    else{ //If file size more than what index block can store, stopped adding and deleted the file itself. 
                        printf("File Too Big!!!! Stopped adding of file. \n");
                        deletefile(file_name);
                        if (deleted == 0 ){ //So won't keep updating the reserved blocks
                            int success = updateReserved(file_name);
                            if (success == 0){
                                printf("Deletion of File %i and its index block failed \n", file_name);
                            }
                            else if(success == 1){
                                deleted = 1;
                                printf("Deletion of File %i and its index block succeeded \n", file_name);
                            }
                        }
                    }
              }
            }
            break;
        case read:
            // if(atoi(data) != 0 && atoi(data)%100 == 0){
            // updateDirectory(0,atoi(data),read);
            // }
            if(atoi(data) != 0){
                findreadfile(data);
            }
            break;
        case delete:
            //if filename
            if(atoi(data) != 0 && atoi(data)%100 == 0){
                deletefile(atoi(data));
                int success = updateReserved(atoi(data));
                updateDirectory(0,atoi(data),delete);
                if (success == 0){
                    printf("Deletion of File %i and its index block failed \n", file_name);
                }
                else if(success == 1){
                    printf("Deletion of File %i and its index block succeeded \n", file_name);
                }
            }
            break;
    }
}

void findreadfile(char f[]){ //Loop through all blocks to find the block with filename, then go to updateDirectory function and read.
    for(int i = 1; i< noOfBlocks; i++){
        for(int j= 0; i< blockSize; j++){
            if(!strcmp(nodes[i][j].data, f)){
                updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename, read);

                return;
            }
        }
    }
}

void checkDirectory(){
    
}


void updateDirectory(int blockNum, int file_name, int choice){
    if (directEntry<superblockSize){
    printf("Inside update Directory\n");
    int done = 0;
    char temp[10];
    char* readtoken;
    int length =0;
    for (int i = 0; i<superblockSize; i++){
        if (choice == add && done != 1){
            printf("inside f statement\n");
            // printf("%i",!strcmp(nodes[0][i].data,"\0"));
            if (!strcmp(nodes[0][i+1].data,"\0")){ //If found the next empty node in directory structure, fill in the data.
                printf("inside strcmp statement\n");
                printf("My I is %i \n", i); 
                strcpy(nodes[0][i+1].data, itoa(file_name, temp, 10));
                strcat(nodes[0][i+1].data, ", ");
                strcat(nodes[0][i+1].data, itoa(blockNum, temp, 10));
                done = 1;   //Indicate done
                // printf("%s\n", nodes[0][i+1].data);
                return;
            }
        }
        else if (choice == read){ // Read entry from directory structure

            if(strcmp(nodes[0][i].data, "\0")){
            strcpy(temp, nodes[0][i].data);
            readtoken = strtok(temp, ", ");
            blockNum = atoi(readtoken);
            printf("first line is %s\n", readtoken);
            printf("first line is %i\n", blockNum);
            
            if(atoi(readtoken) == file_name){
                readtoken = strtok(NULL, " ");
                blockNum = atoi(readtoken);
                printf("first line is %s\n", readtoken);
                printf("block num is %i\n", blockNum);
             }
            }
        }
        else if (choice == delete && done != 1){ // Delete entry structure
            //Splitting up data for each line of directory structure
            strcpy(temp, nodes[0][i].data);
            readtoken = strtok(temp, ", ");
            if(atoi(readtoken) == file_name){
                strcpy(nodes[0][i].data, "\0");
                deletefile(file_name);
            }
        }
        //     switch(choice){
        //     case add:
        //         if (done == FALSE){
        //             if (!strcmp(nodes[0][i + 1].data,"\0")){ //If found the next empty node in directory structure, fill in the data.
        //                 strcpy(nodes[0][i + 1].data, itoa(file_name, temp, 10));
        //                 strcat(nodes[0][i + 1].data, ", ");
        //                 strcat(nodes[0][i + 1].data, itoa(blockNum, temp, 10));
        //                 done = TRUE;
        //                 return;
        //             }
        //         }
        //         break;
        //     case read:
        //         if(strcmp(nodes[0][i].data, "\0")){
        //             strcpy(temp, nodes[0][i].data);
        //             readtoken = strtok(temp, ", ");
        //             printf("first part is %s\n", readtoken);
        //             blockNum = atoi(readtoken);
        //             printf("num is %i\n", blockNum);
                    
        //             if(atoi(readtoken) == file_name){
        //                 readtoken = strtok(NULL, " ");
        //                 blockNum = atoi(readtoken);
        //                 // printreadfile(blockNum, length);
        //                 printf("first part is %s\n", readtoken);
        //                 printf("num is %i\n", blockNum);
        //             }
        //         }              
        //         break;
        //     case delete:
        //         // if(strcmp(nodes[0][i].data, "\0")){
        //         //     strcpy(temp, nodes[0][i].data);
        //         //     readtoken = strtok(temp, ", ");
        //         //     if(atoi(readtoken) == file_name){
        //         //         strcpy(nodes[0][i].data, "\0");
        //         //         deletefile(file_name);
        //         //     }
        //         // }
        //         if (strcmp(nodes[0][i].data, "\0")){
        //             strcpy(temp, nodes[0][i].data);
        //             readtoken = strtok(temp, ", ");
        //             if(atoi(readtoken) == file_name){
        //                 strcpy(nodes[0][i].data, "\0");
        //                 deletefile(file_name);
        //             }
        //         }
        //         break;
        // }
     }
    }
    else if(directEntry<superblockSize){
        

    }
}

int updateReserved(int fileName){
    int unreservedBlock = 1000; //Just set a number for validation later
    for(int i = 1; i < noOfBlocks; i++){  
        if(nodes[i][1].reserved == fileName){    //Loop through all the block names and if it is the same as the selected block to unreserve.
            unreservedBlock = i;   //select the block to unreserve as the found i.

            break;
        }
    }
    if (unreservedBlock != 1000) {
        for (int j =0; j< blockSize; j++){
            if(nodes[unreservedBlock][j].reserved == fileName){    //Loop through all the nodes in block and if it is the same as the selected block to unreserve.
            nodes[unreservedBlock][j].reserved = 0;   // set block attribute "reserved" to 0.
            strcpy(nodes[unreservedBlock][j].data, "\0"); //Set data as null
            }
        }
        updatefsm(unreservedBlock);
        directEntry--;
        return 1;
    }
    else{
        return 0;
    }
 }




int findNewBlock(){
    int blockNum = randomBlock();
    // Check that block is unused from FSM & manually check empty.
    if ((checkFree(blockNum) == TRUE) && (nodes[blockNum][0].reserved == 0) && strcmp("\0", nodes[blockNum][0].data) == 0 && (nodes[blockNum][0].filename == 0)){
        return blockNum;
    }
    else{
        findNewBlock();
    }
}
void reserveBlock(int fileName){
    int blockNum = findNewBlock();
    for (int i =0; i<noOfBlocks; i++){
        nodes[blockNum][i].reserved = file_name;
        strcpy(nodes[blockNum][i].data,"-1" );
    }
    updatefsm(blockNum);
    updateDirectory(blockNum, file_name, 1);
    directEntry++;
}

void savetofile(int f, char d[], int blockNum){
    // printf("Inside Save to File: %i \n", blockNum);
    // printf("Inside savetofile, filename: %i, data %s : \n", f, d);
    if(checkFree(blockNum) == TRUE){
        updatefsm(blockNum);
        updateIndexBlock(f, blockNum);
    }

    nodes[blockNum][columncount].filename = f;
    strcpy(nodes[blockNum][columncount].data, d);
    printf("%i \n", nodes[blockNum][columncount].filename);
    printf("%s \n", nodes[blockNum][columncount].data);
}

void updateIndexBlock(int f, int blockNum){
    char temp[3];
    for (int i=0; i<noOfBlocks; i++){
        for (int j=0; j<blockSize; j++){
            if (nodes[i][j].reserved == f){
                if (strcmp(nodes[i][j].data, "-1") == 0){
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

void deletefile(int f){
    int delblock = 0;
    for(int i = 1; i < noOfBlocks; i++){  
        if(nodes[i][1].filename == f){    //Loop through all the block names and if it is the same as the selected block to delete.
            delblock = i;   //select the block to delete as the found i.
            deleteblock(delblock, f);  //Parse into deleteblock
        }
    }
}

void deleteblock(int block, int f){
    for(int j = 0; j< blockSize; j++){
        if(nodes[block][j].filename == f){ //Loop through selected block 
            nodes[block][j].filename = 0; //Set file name as 0
            strcpy(nodes[block][j].data, "\0"); //Set data as reserved
        }
    }
    updatefsm(block);
    // updateDirectory(block, file_name, 1);
}

int randomBlock()
{
    int _randomBlock = rand() % noOfBlocks;
    return _randomBlock;
}

void printallocation(){
    for(int i = 0; i < noOfBlocks; i++){
        if (i ==0){
        for(int k = 0; k < superblockSize; k++){
            printf("%d\t%d\t%d\t%s\t%i \n", nodes[i][k].index,nodes[i][k].blockNo, nodes[i][k].filename,nodes[i][k].data,nodes[i][k].reserved);    
        }
        }
        else {
            for(int j = 0; j < blockSize; j++){
            printf("%d\t%d\t%d\t%s\t%i \n", nodes[i][j].index,nodes[i][j].blockNo, nodes[i][j].filename,nodes[i][j].data,nodes[i][j].reserved);
            }
        }
    }
    printf("%s", fsm);

}

void readCSV(char input[]){
    register int count=0;
    FILE *fp = fopen(input,"r");

    char buffer[1024];
        while(fgets(buffer, 1024, (FILE*)fp)){
        count++;
        if(count != 0){
            getData(buffer);
        }
    }

    fclose(fp);
}

void getData(char buffer[])
{
   char *token = strtok(buffer,",");
   int counter=0;
   while(token) 
   {
        //printf("%s\n",token);
        //strcpy(CSV_Data[counter], token);
       // printf(CSV_Data[counter]);
        allocation(token);
        token = strtok(NULL,", ");


        counter++; 
   }	  
}