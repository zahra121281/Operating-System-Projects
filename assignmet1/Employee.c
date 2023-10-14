#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Employee
{
    char* Name; 
    struct Employee* Next ; 
} employee ;

employee* add(employee* head , char Name[] , int pos )
{
    int i=1 ; 
    employee* current = head ; 
    employee* new = (employee*) malloc(sizeof(employee)) ; 
    //set new name 
    int m = strlen(Name) +1 ;
    new->Name = (char*) malloc(m*sizeof(char)) ; 
    memcpy(new->Name, Name,m*sizeof(char)); 
    if(pos ==0 )
    {
        new->Next = head ; 
        head = new ; 
        return head; 
    }
    while (current!= NULL )
    {
        if(i ==pos )
        {
            new->Next = current->Next ; 
            current->Next = new ; 
            return head; 
        }
        current = current->Next ; 
        i++ ; 
    }
    
}

employee* delete(employee* head , char Name[] )
{
    employee* current = (employee*) malloc(sizeof(employee)); 
    current->Next = head ; 
    char* headname = head->Name ; 
    while (current != NULL)
    {
        if(strcmp(current->Next->Name,Name) == 0 )
        {
            employee* tofree = current->Next ; 
            current->Next = current->Next->Next ; 
            free(tofree ) ; 
            if(strcmp(headname,Name) == 0 )
                head = current->Next ; 
            return head; 
        }
        current =current->Next ; 
    }
    
}

void swap(employee* head , char Name1[] , char Name2[] )
{
    employee* current = (employee*) malloc(sizeof(employee)); 
    current = head ; 
    char *headname = head->Name ; 
    while (current != NULL)
    {
        if(strcmp(current->Name,Name1) == 0 )
        {
            current->Name = Name2 ; 
        }
        else if(strcmp(current->Name,Name2) == 0 )
        {
            current->Name = Name1 ; 
        }
        current =current->Next ; 

    }
}

int main()
{
    int num ; 
    scanf("%d" , &num ); 
    employee* head ;
    employee* next =(employee*) malloc(sizeof(employee))  ; 
    employee* current ; 
    head = next  ; 
    int i ;
    for(i=0 ; i < num ; i++ )
    {
        current = next ; 
        char name[40] ; 
        scanf("%s" , &name ) ;
        int m = strlen(name) +1 ;
        current->Name = (char*) malloc(m*sizeof(char)) ; 
        memcpy(current->Name, name,m*sizeof(char)); 
        next = (employee*) malloc(sizeof(employee)) ; 
        current->Next = next ;
    }
    current->Next = NULL ; 
    current = head  ; 
    int changes  ; 
    scanf("%d" , &changes ) ; 
    for(i=0 ; i < changes ; i++ )
    {
        char command[10] ; 
        scanf("%s" , &command ) ; 
        if ( strcmp(command , "add")==0 ) 
        {
            int place ; char name[25] ;
            scanf("%s %d" , &name , &place )  ;  
            // printf("%s %d" , name , place )  ; 
            head = add(head , name , place ) ; 
        }
        else if (strcmp( command ,"delete")==0 )
        {
            char name[25] ;
            scanf("%s" , &name )  ; 
            // printf("%s" , name )  ; 
            head=delete(head , name ) ;  
        }
        else //swap 
        {
            char name1[25] ;char name2[25] ;
            scanf("%s %s" , &name1,&name2 )  ;  
            // printf("%s %s" , name1,name2 )  ;  
            swap(head , name1 , name2 ) ; 
        }
        
    }

    current = head ; 
    while(current != NULL)
    {
        printf("%s " , current->Name ) ; 
        current = current->Next ; 
    }
    
    
}