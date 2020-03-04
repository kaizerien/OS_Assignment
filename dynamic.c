
//Another
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INITIAL_CAPACITY 1
// void push(char *arr, int *capacity)
// {
//     //if(*size > *capacity){
//     printf("%d", sizeof(arr));
//     int a = 26%4;
//     int b = (int)floor(26/4);

//     realloc(arr, sizeof(arr) * b + a); //Size of a pointer is 4 
//     *capacity = sizeof(arr) * b + a;

// }
int main()
{
    char *arr = calloc(26, INITIAL_CAPACITY * sizeof(char)); //Size of char = 1 bytes
    for(int i = 0; i < 27; i++)
    {
        arr[i] = 't';
    }

    printf("%s", &arr);
}