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
            //printf("+");
            if(atoi(data) != 0 && atoi(data)%100 == 0){
                //strcpy(file_name, data);
                file_name = atoi(data);
            }else{
                if(atoi(data)!=0){
                    savetofile(file_name, data);
                }
            }
            break;
        case read:
            //printf("=");
            break;
        case delete:
            //printf("-");
            if(atoi(data) != 0 && atoi(data)%100 == 0){
                //strcpy(file_name, data);
            }
            break;
    }
}

void savetofile(int f, char d[]){
    for(int i = 1; i< noOfBlocks; i++){
        for(int j = 0; j < blockSize ; j ++)
            if(checkspace(i, j) == TRUE){
                blocks[i][j].filename = f;
                strcpy(blocks[i][j].data, d);
            return;
        }
    }
}

int checkspace(int n, int k){
    if (strcmp(blocks[n][k].data, "/0") && blocks[n][k].filename == 0){
        return TRUE;
    }else{
        return FALSE;
    }
}

void printallocation(){
    for(int i = 0; i < noOfBlocks; i++){
        for(int j = 0; j < blockSize ; j++)
            if(blocks[i][j].filename != 0){
                printf("Node Index: %i\n", blocks[i][j].index);
                printf("Filename: %i\n", blocks[i][j].filename);
                printf("Data: %s\n", blocks[i][j].data);
        }
    }
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