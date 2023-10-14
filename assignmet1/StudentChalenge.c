#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Student
{
    char* Name;
    int Score ; 
    int Age ; 
} ;

struct Group
{
    int group1 ; 
    int group2 ; 
    double average ; 
};


void StudentToString(struct Student s )
{
    printf("%s %d %d" , s.Name , s.Age ,s.Score); 
}

struct Group* MakeGroups(struct Student* students , int minIndex , int Snum)
{
    int Gnum = Snum/2 ; 
    int j = Snum -1 ; 
    int counter = 0 ; 
    struct Group * groups = (struct Group*) malloc( Gnum * sizeof(struct Group)) ; 
    int i ;
    for(i=0 ;i < Snum && counter < Gnum ; i++ )
    {
        if( j == minIndex )
        {
            j-- ; 
            i-- ; 
            continue; 
        }
        if( i!= minIndex )
        {
            struct Group grp  ; 
            grp.group1 = i ; grp.group2= j ; grp.average = (students[i].Score+students[j].Score )/2 ; 
            groups[counter] = grp ; 
            j-- ; 
            counter++ ; 
        }
    }
    return groups ; 
}

void SortGroups(struct Group* groups , int Gnum )
{
    int i , j ; 
    for(i=0; i<Gnum; i++)
    {
        for(j=i+1; j<Gnum ; j++) { 
            struct Group temp ; 
            if(groups[i].average >groups[j].average )
            {
                temp = groups[i];
                groups[i] = groups[j];
                groups[j] = temp;
            }
        }
    }
}

void PrintGroups(struct Group* groups ,struct Student* students , int Gnum ) 
{
    int i ;  
    for(i=Gnum-1 ; i >= 0 ; i-- )
    {
        int n = groups[i].group1; int m = groups[i].group2 ;
        //printf("i: %d g1: %d g2: %d average: %f \n" ,i, n , m, groups[i].average ) ;  
        StudentToString(students[n]) ; printf(", ") ; 
        StudentToString(students[m]) ; printf("\n") ; 
       
    }
}
int main()
{
    /*
        sample input :
        5
        Ava 30 200
        Ethan 29 75
        Mia 23 180
        Noah 35 250
        Isabella 27 160
        5
        Ava 30 200
        Ethan 29 75
        Mia 23 180
        Noah 35 250
        Isabella 27 160
        ****************
        sample output : 
        Mia 23 180, Noah 35 250
        Ava 30 200, Isabella 27 160  
    */
   
    // get the input numbers ; 
    int num ; 
    scanf("%d" , &num ) ; 
    int i=0 ; 

    // make an array of students : 
    
    struct Student* students ; 
    students = (struct Student*) malloc(num*sizeof(struct Student));
    int minScore = 900000000 ;
    int minIndex =-1 ; 
    for(i=0 ; i< num ; i++ )
    {
        struct Student student  ; 
        char name[30] ; int age ; int score ; 
        scanf("%s %d %d" , &name , &age ,&score) ; 
        student.Age = age ; student.Score= score ; 
        int m = strlen(name) +1 ;
        student.Name = (char*) malloc(m* sizeof(char)) ; 
        memcpy(student.Name, name,m*sizeof(char)); 
        students[i] = student ;
        if( score <= minScore )
        {
            minScore = score ; 
            minIndex = i ; 
        }  
       //printf("%s \n" , student.Name ) ; 
    }
    if ( num%2 == 0 )
        minIndex = -1 ; 
    
    struct Group* grps =  MakeGroups(students , minIndex , num ) ; 
    SortGroups(grps , num/2 ) ; 
    PrintGroups( grps , students , num/2 ) ; 
    return 0 ; 
}
