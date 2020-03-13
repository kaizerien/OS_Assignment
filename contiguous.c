#include "contiguous.h"


int contiguous(void){

    printf("Enter name of .csv file to read from\n");
    //scanf("%s", &input);
    readCSV("SampleCSV.csv");

    printallocation();
}

void allocation(char *data){
    char choice;

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
            }else{
                if(atoi(data)!=0){
                    filesize +=1;
                    if(filesize > blockSize){
                        if(checkfree(curBlock+1) == TRUE){
                            curBlock = checkfsm();
                            savetofile(file_name, data);
                        }                           
                    }else{
                        savetofile(file_name, data);
                    }
                    
                }
            }
            break;
        case read:
            if(atoi(data) != 0){
                readfile(data);
            }
            break;
        case delete:
            if(atoi(data) != 0 && atoi(data)%100 == 0){
                deletefile(atoi(data));
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
    if (strcmp(nodes[n][k].data, "\0") == 0 && nodes[n][k].filename == 0){
        return TRUE;
    }else{
        return FALSE;
    }
}

void readfile(char f[]){
    for(int i = 1; i< noOfBlocks; i++){
        for(int j= 0; i< blockSize; j++){
            if(strcmp(nodes[i][j].data, f) == 0){
                printf("Block No: %i\n", (nodes[i][j].blockNo));
                printf("Filename: %i\n", (nodes[i][j].filename));
                return;
            }
        }
    }
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
        for(int j = 0; j < blockSize; j++){
            if(nodes[i][j].filename != 0){
                printf("Node Index: %i\n", nodes[i][j].index);
                printf("Block No: %i\n", nodes[i][j].blockNo);
                printf("Filename: %i\n", nodes[i][j].filename);
                printf("Data: %s\n", nodes[i][j].data);
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
        //printf("%s\n",token);
        allocation(token);
        token = strtok(NULL,", ");


        counter++; 
   }	  
}