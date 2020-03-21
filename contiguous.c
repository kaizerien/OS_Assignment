#include "contiguous.h"
#include "vcb.c"
#include "shared.h"

int contiguous(void){

    printf("Enter name of .csv file to read from\n");
    scanf("%s", &input);
    //Enter .CSV file to run
    readCSV(input);

    //Print everything at end of program
    printallocation();
}

void contiguousAllocation(char *data){
    int file_name;

    if(!strcmp(data, "add")){
        choice = add;
    }else if(!strcmp(data, "read")){
        choice = read;
    }else if(!strcmp(data, "delete")){
        choice = delete;
    }
    switch(choice){
        case add:
            if(atoi(data) != 0 && atoi(data)%100 == 0){
                file_name = atoi(data);
                //Check for avaliable block
                if(checkfsm()!= -1){
                    curBlock = checkfsm();
                    printf("Adding file %i and found free block %i\n", file_name, curBlock);
                    filesize = 0;
                    filelength = 1;
                    //Add file to directory
                    updateDirectory(curBlock, file_name, add);
                    diskspace = TRUE;
                }else{
                    printf("Not enough space on disk");
                    diskspace = FALSE;
                }

            }else{
                if(atoi(data)!=0 && diskspace == TRUE){
                    filesize +=1;
                    //If filesize too big for one block
                    if(filesize > blockSize){
                        if(checkfree(curBlock+1) == TRUE){ //Ensure next block is free because contiguous
                            curBlock = checkfsm();
                            printf("Adding file %i and found free block %i\n", file_name, curBlock);
                            filesize = 0;
                            filelength +=1;
                            savetofile(file_name, data);
                            //Overwrite old directory entry to update length
                            updateDirectory(((curBlock + 1)-filelength), file_name, overwrite);
                        }else{
                            printf("Disk not enough space for file!");
                            finddeletefile(atoi(file_name));
                        }                      
                    }else{
                        //Save data to block
                        savetofile(file_name, data);
                    }
                    
                }
            }
            vcbfunc(nodes);
            strcpy(nodes[0][0].data, vcbString);
            break;
        case read:
            if(atoi(data) != 0){
                //Find if file exist
                findreadfile(data);
            }
            vcbfunc(nodes);
            strcpy(nodes[0][0].data, vcbString);
            break;
        case delete:
            if(atoi(data) != 0 && atoi(data)%100 == 0){
                //Find if file exist
                finddeletefile(atoi(data));
            }
            vcbfunc(nodes);
            strcpy(nodes[0][0].data, vcbString);
            break;
    }
}

void savetofile(int f, char d[]){
    if(checkfree(curBlock) == TRUE){
        updatefsm(curBlock);
    }
    for(int i = 0; i < blockSize; i++){
        if(checkspace(curBlock, i) == TRUE){
            nodes[curBlock][i].filename = f;
            strcpy(nodes[curBlock][i].data, d);
            printf("%d\t%d\t%d\t%s\n", nodes[curBlock][i].index,nodes[curBlock][i].blockNo, nodes[curBlock][i].filename,nodes[curBlock][i].data);
            return;
        }
    }
}

// int checkspace(int n, int k){
//     if (!strcmp(nodes[n][k].data, "\0") && nodes[n][k].filename == 0){
//         return TRUE;
//     }else{
//         return FALSE;
//     }
// }

// //Code to access directory
// void updateDirectory(int blockNum, int filename,int state){
//     int done = FALSE;
//     char temp[10];
//     char* readtoken;
//     for (int i = 1; i<superblockSize; i++){
//         switch(state){
//             case add:
//                 if (done == FALSE){
//                     //If found the next empty node in directory structure, fill in the data.
//                     if (!strcmp(nodes[0][i].data,"\0")){
//                         strcpy(nodes[0][i].data, itoa(filename, temp, 10));
//                         strcat(nodes[0][i].data, ", ");
//                         strcat(nodes[0][i].data, itoa(blockNum, temp, 10));
//                         strcat(nodes[0][i].data, ", ");
//                         strcat(nodes[0][i].data, itoa(filelength, temp, 10));
//                         done = TRUE;
//                         return;
//                     }
//                 }
//                 break;
//             case read:
//                 //Search directory for file information(name, length)
//                 if(strcmp(nodes[0][i].data, "\0")){
//                     strcpy(temp, nodes[0][i].data);
//                     readtoken = strtok(temp, ", ");
//                     if(atoi(readtoken) == filename){
//                         readtoken = strtok(NULL, ", ");
//                         blockNum = atoi(readtoken);
//                         readtoken = strtok(NULL, " ");
//                         filelength = atoi(readtoken);
//                         printreadfile(blockNum, filename, filelength);
//                     }
//                 }              
//                 break;
//             case delete:
//                 //Search directory and remove file directory entry
//                 if(strcmp(nodes[0][i].data, "\0")){
//                     strcpy(temp, nodes[0][i].data);
//                     readtoken = strtok(temp, ", ");
//                     if(atoi(readtoken) == filename){
//                         strcpy(nodes[0][i].data, "\0");
//                         //Function to delete all entry under this filename
//                         deletecontent(filename);
//                     }
//                 }
//                 break;
//             case overwrite:
//                 //For overwrite directory entry when file cannot fit into a single block
//                 if(strcmp(nodes[0][i].data, "\0")){
//                     strcpy(temp, nodes[0][i].data);
//                     readtoken = strtok(temp, ", ");
//                     if(atoi(readtoken) == filename){
//                         strcpy(nodes[0][i].data, "\0");         
//                         strcpy(nodes[0][i].data, itoa(filename, temp, 10));        
//                         strcat(nodes[0][i].data, ", ");
//                         strcat(nodes[0][i].data, itoa(blockNum, temp, 10));
//                         strcat(nodes[0][i].data, ", ");
//                         strcat(nodes[0][i].data, itoa(filelength, temp, 10));
//                         done = TRUE;
//                         return;
//                     }
//                 }
//                 break;

//         }
//     }
//  }

// //Func to check if file to be read exists
// void findreadfile(char f[]){
//     int found = TRUE;
//     for(int i = 1; i< noOfBlocks; i++){
//         for(int j= 0; j< blockSize; j++){
//                 if(!strcmp(nodes[i][j].data, f)){ //If user wants to read by file data
//                     updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename,read);
//                     return;
//                 }else if((atoi(f)) == nodes[i][j].filename){ //If user want tor ead by file name
//                     updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename,read);
//                     return;
//                 }else{
//                     found = FALSE;
//                 }
//         }
//     }
//     if(found == FALSE){
//         printf("File %s not found on disk\n", f);
//     }
// }

// //Func to output the file user wants to read
// void printreadfile(int blockNo, int filename, int length){
//     printf("Read file %i from block %i\n", filename, blockNo);
//     for(int i = 0; i<blockSize; i++){
//         if(strcmp(nodes[blockNo][i].data, "\0")){
//             printf("%d\t%d\t%d\t%s\n", nodes[blockNo][i].index,nodes[blockNo][i].blockNo, nodes[blockNo][i].filename,nodes[blockNo][i].data);
//         }
//     }
//     //Recursively access other blocks if file uses more then one block
//     if (length > 1){
//         printreadfile(blockNo+1, filename, length - 1);
//     }
// }

// //Func to check if file to be deleted exist
// void finddeletefile(int f){
//     int found = TRUE;
//     for(int i = 1; i < noOfBlocks; i++){
//         for(int j = 0; j< blockSize; j++){
//             if(nodes[i][j].filename == f){
//                 if(checkfree(i) == FALSE){
//                     updatefsm(i);
//                     printf("Delete file %i and freed Block %i\n", f, i);
//                     //Update directory to remove directory entry of file
//                     updateDirectory(0,f, delete);
//                     return;
//                 }
//                 return;
//             }else{
//                 found = FALSE;
//             }
//         }
//     }
//     if(found == FALSE){
//         printf("File %i not found on disk\n", f);
//     }
// }

// //Recursive func to delete entry under filename
// void deletecontent(int f){
//     for(int i = 1; i < noOfBlocks; i++){
//         for(int j = 0; j< blockSize; j++){
//             if(nodes[i][j].filename == f){
//                 nodes[i][j].filename = 0;
//                 strcpy(nodes[i][j].data, "\0");
//                 deletecontent(f);
//                 return;
//             }
//         }
//     }
// }

//Func to print all blocks
// void printallocation(){
//     printf("Final allocation\n");
//     printf("Index\tBlock\tFilename\tData\n");
//     for(int i = 0; i < noOfBlocks; i++){
//         if(i == 0){
//             for(int k = 0; k < superblockSize; k++){ //Prints block 0 aka superblock
//                 printf("%d\t%d\t%d\t%s\n", nodes[i][k].index,nodes[i][k].blockNo, nodes[i][k].filename,nodes[i][k].data);
//             }
//         }else{
//             for(int j = 0; j < blockSize; j++){ //Print rest of the block
//                 printf("%d\t%d\t%d\t%s\n", nodes[i][j].index,nodes[i][j].blockNo, nodes[i][j].filename,nodes[i][j].data);
//             }
//         }
        
//     }
//     printf("FSM: %s\n", fsm);
// }

//Func to read .CSV files
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

//Func to input read data into allocation method
void getData(char buffer[])
{
   char *token = strtok(buffer,", ");
   char copy[1000][1000];
   int counter = 0;
   while(token) 
   {
        strcpy(copy[counter], token);
        //contiguousAllocation(token);
        token = strtok(NULL,", ");
        counter++;
   }	 
   for(int i = 0; i < counter; i++){
       contiguousAllocation(copy[i]);
    } 
}