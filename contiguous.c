#include "contiguous.h"

unsigned char input[20];

enum allstate{add = 1, read = 2, delete = 3}state;
int contiguous(){

    printf("Enter name of .csv file to read from\n");
    //scanf("%s", &input);
    readCSV("SampleCSV.csv");


}

void allocation(char *data){
    char choice;
    
    printf("%s\n",data);

    if(!strcmp(data, "add")){
        choice = add;
    }else if(!strcmp(data, "read")){
        choice = read;
    }else if(!strcmp(data, "delete")){
        choice = delete;
    }
    switch(choice){
        case add:
            printf("+");
            break;
        case read:
            printf("=");\
            break;
        case delete:
            printf("-");
            break;
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