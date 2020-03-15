#include "contiguous.h"


int contiguous(void){

    printf("Enter name of .csv file to read from\n");
    //scanf("%s", &input);
    readCSV("SampleCSV.csv");

    printallocation();
}

void allocation(char *data){
    int file_name;
    char choice;
    //int filelength = 0;

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
                curBlock = checkfsm();
                filesize = 0;
                filelength = 1;
                updateDirectory(curBlock, file_name, add);

            }else{
                if(atoi(data)!=0){
                    filesize +=1;
                    if(filesize > blockSize){
                        if(checkfree(curBlock+1) == TRUE){
                            curBlock = checkfsm();
                            filesize = 0;
                            filelength +=1;
                            savetofile(file_name, data);
                            updateDirectory(((curBlock + 1)-filelength), file_name, overwrite);
                        }else{
                            
                        }                           
                    }else{
                        savetofile(file_name, data);
                    }
                    
                }
            }
            break;
        case read:
            if(atoi(data) != 0){
                findreadfile(data);
            }
            break;
        case delete:
            if(atoi(data) != 0 && atoi(data)%100 == 0){
                updateDirectory(0,atoi(data),delete);
            }
            break;
    }
}

void savetofile(int f, char d[]){
    if(checkfree(curBlock) == TRUE){
        updatefsm(curBlock);
    }
    for(int j = 0; j < blockSize; j++){
        if(checkspace(curBlock, j) == TRUE){
            nodes[curBlock][j].filename = f;
            strcpy(nodes[curBlock][j].data, d);
            return;
        }
    }
}

int checkspace(int n, int k){
    if (!strcmp(nodes[n][k].data, "\0") && nodes[n][k].filename == 0){
        return TRUE;
    }else{
        return FALSE;
    }
}

void updateDirectory(int blockNum, int filename,int state){
    int done = FALSE;
    char temp[10];
    char* readtoken;
    for (int i = 0; i<superblockSize; i++){
        switch(state){
            case add:
                if (done == FALSE){
                    if (!strcmp(nodes[0][i + 1].data,"\0")){ //If found the next empty node in directory structure, fill in the data.
                        strcpy(nodes[0][i + 1].data, itoa(filename, temp, 10));
                        strcat(nodes[0][i + 1].data, ", ");
                        strcat(nodes[0][i + 1].data, itoa(blockNum, temp, 10));
                        strcat(nodes[0][i + 1].data, ", ");
                        strcat(nodes[0][i + 1].data, itoa(filelength, temp, 10));
                        done = TRUE;
                        return;
                    }
                }
                break;
            case read:
                if(strcmp(nodes[0][i].data, "\0")){
                    strcpy(temp, nodes[0][i].data);
                    readtoken = strtok(temp, ", ");
                    if(atoi(readtoken) == filename){
                    //     while(readtoken != NULL){
                    //         printf("%s\n",readtoken);
                    //         readtoken = strtok(NULL, " ");
                        readtoken = strtok(NULL, ", ");
                        blockNum = atoi(readtoken);
                        readtoken = strtok(NULL, " ");
                        filelength = atoi(readtoken);
                        printreadfile(blockNum, filelength);
                    }
                }              
                break;
            case delete:
                if(strcmp(nodes[0][i].data, "\0")){
                    strcpy(temp, nodes[0][i].data);
                    readtoken = strtok(temp, ", ");
                    if(atoi(readtoken) == filename){
                        strcpy(nodes[0][i].data, "\0");
                        deletefile(filename);
                    }
                }
                break;
            case overwrite:
                if(strcmp(nodes[0][i].data, "\0")){
                    strcpy(temp, nodes[0][i].data);
                    readtoken = strtok(temp, ", ");
                    if(atoi(readtoken) == filename){
                        strcpy(nodes[0][i].data, "\0");
                        //if (done == FALSE){            
                            strcpy(nodes[0][i].data, itoa(filename, temp, 10));        
                            strcat(nodes[0][i].data, ", ");
                            strcat(nodes[0][i].data, itoa(blockNum, temp, 10));
                            strcat(nodes[0][i].data, ", ");
                            strcat(nodes[0][i].data, itoa(filelength, temp, 10));
                            done = TRUE;
                            return;
                        //}
                    }
                }
                break;

        }
    }
 }

void findreadfile(char f[]){
    for(int i = 1; i< noOfBlocks; i++){
        for(int j= 0; i< blockSize; j++){
            if(!strcmp(nodes[i][j].data, f)){
                updateDirectory(nodes[i][j].blockNo, nodes[i][j].filename,read);
                //readfilename = nodes[i][j].filename;
                //printf("Block No: %i\n", (nodes[i][j].blockNo));
                //printf("Filename: %i\n", (nodes[i][j].filename));
                return;
            }
        }
    }
}

void printreadfile(int blockNo, int length){
    printf("%i\n%i\n", blockNo, length);
}

void deletefile(int f){
    for(int i = 1; i < noOfBlocks; i++){
        for(int j = 0; j< blockSize; j++){
            if(nodes[i][j].filename == f){
                nodes[i][j].filename = 0;
                strcpy(nodes[i][j].data, "\0");
                if(checkfree(i) == FALSE){
                    updatefsm(i);
                }
            }
        }
    }
}

void printallocation(){
    for(int i = 0; i < noOfBlocks; i++){
        if(i == 0){
            for(int k = 0; k < superblockSize; k++){
                printf("%d\t%d\t%d\t%s\n", nodes[i][k].index,nodes[i][k].blockNo, nodes[i][k].filename,nodes[i][k].data);
            }
        }else{
            for(int j = 0; j < blockSize; j++){
                printf("%d\t%d\t%d\t%s\n", nodes[i][j].index,nodes[i][j].blockNo, nodes[i][j].filename,nodes[i][j].data);
            }
        }
        
    }
    printf("FSM: %s\n", fsm);
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
        allocation(token);
        token = strtok(NULL,", ");


        counter++; 
   }	  
}