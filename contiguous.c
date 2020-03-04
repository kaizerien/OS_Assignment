#include "contiguous.h"

int contiguous(void){

    printf("Enter name of .csv file to read from\n");
    //scanf("%s", &input);
    readCSV("SampleCSV.csv");

    printallocation();
}

void allocation(char *data){
    char choice;
    
    //printf("%s\n",data);

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
    for(int i = 0; i< MAX_BLOCK; i++){
        if(checkspace(i) == TRUE){
            node[i].filename = f;
            strcpy(node[i].data, d);
        return;
        }
    }
}

int checkspace(int n){
    if (strcmp(node[n].data, "/0") && node[n].filename == 0){
        return TRUE;
    }else{
        return FALSE;
    }
}

void printallocation(){
    for(int i = 0; i < MAX_BLOCK; i++){
        if(node[i].filename != 0){
            printf("Node Index: %i\n", node[i].index);
            printf("Block No: %i\n", node[i].blockNo);
            printf("Filename: %i\n", node[i].filename);
            printf("Data: %s\n", node[i].data);
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