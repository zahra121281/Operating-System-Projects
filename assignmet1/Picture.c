#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void PrintMatrix(int size , int* matrix )
{
    int i , j ; 
    for(i=0 ; i< size ; i++ )
    {
        for(j=0 ; j < size ; j++ )
        {
            printf("%d " , matrix[i*size +j ]) ; 
        }
        printf("\n")  ;
    }
}

int* Rotate90(int size , int *matrix)
{
    int i, j  ; 
    int* new_matrix = (int*) malloc(sizeof(int)*size*size);  
    for(i=0 ; i< size ; i++ )
    {
        for(j=0 ; j < size ; j++ )
        {
            new_matrix[ j*size + (size-1-i) ] = matrix[i*size+j]; 
        }
    }
    return new_matrix ; 
}

int main()
{
    int size ; 
    scanf("%d" , &size ) ;  
    int* pixels = (int*) malloc(sizeof(int)*size*size);  
    int i , j ; 
    for(i=0 ; i< size*size ; i++ )
    {
        int pixel ; 
        scanf("%d" , &pixel) ; 
        pixels[i] = pixel ; 
    } 
    //270
    if(pixels[(size-1)*size] == 0 )
    {
        pixels = Rotate90(size , pixels ) ; 
        PrintMatrix(size , pixels ) ; 
    }
    // 90 
    else if ( pixels[size-1] == 0 )
    {
        pixels = Rotate90(size , pixels ) ; 
        pixels = Rotate90(size , pixels ) ; 
        pixels = Rotate90(size , pixels ) ; 
        PrintMatrix(size , pixels ) ; 
    }
    // 180 
    else if(pixels[size*size-1] == 0 )
    {
        pixels = Rotate90(size , pixels ) ; 
        pixels = Rotate90(size , pixels ) ; 
        PrintMatrix(size , pixels ) ; 
    }
    // 0 , 360 
    else
    {
        PrintMatrix(size , pixels ) ; 
    }
    return 0; 
}