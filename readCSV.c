#include "readCSV.h"
#include "linked.c"

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
        strcpy(CSV_Data[counter], token);
       // printf(CSV_Data[counter]);
        token = strtok(NULL,", ");

        linked_allocation(CSV_Data[counter],counter);

        counter++; 
   }	  
}