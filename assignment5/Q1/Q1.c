#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 1000000


int mypipe[2][2];
int line_num ; 


int main(int argc , char** argv){
    int randnum = 0;
    
    pipe(mypipe[0]);
    pipe(mypipe[1]);
    
    int pid1 = fork();
    if(pid1 == 0){
        int sum = 0;
        int l = 0 ; 
        read(mypipe[0][0], &l, sizeof(int));
        printf("in child 1 num line %d\n" , l ) ; 
        int num = 0 ; 
        
        for(int i=0 ; i< l ; i++ ){
            read(mypipe[0][0], &num, sizeof(int));
            printf("number read in child 1 :%d\n" , num) ; 
            sum += num ;
        }
        printf("child 1 is writing sum : %d..\n" , sum ) ; 
        write(mypipe[0][1], &sum, sizeof(int));
        
        exit(0);
    }


    int pid2 = fork();
    if(pid2 == 0){
        int min = MAX ;
        int num = 0 ; 
        int l = 0 ; 
        read(mypipe[1][0], &l, sizeof(int));
        printf("in child 2 num line %d\n" , l ) ; 
        
        for(int i=0 ; i< l ; i++ ){
            read(mypipe[1][0], &num, sizeof(int));
            printf("number read in child 2 :%d\n" , num) ; 
            if(num < min ) 
                min = num ; 
        }
        printf("child 2 is writing.. min : %d\n" , min ) ; 
        write(mypipe[1][1], &min , sizeof(int));
        exit(0);

    }

    FILE *file;
    char * source = argv[1] ;
    file = fopen( source , "r");
    
    if (file == NULL) {
        perror("Error opening file\n");
        exit(1) ; 
    }

    fscanf(file, "%d", &line_num) ; 
    printf("parrent :%d\n" , line_num) ; 
    write(mypipe[1][1], &line_num, sizeof(int));
    write(mypipe[0][1], &line_num, sizeof(int));
    //sleep(2) ; 
    int min =0 ; 
    int mean = 0 ; 
    for(int i=0 ; i< line_num ; i++ )
    {
        int number =0 ; 
        if ( fscanf(file, "%d", &number) != 1 )
        {
            printf("number of lines are fwer than expected!!\n") ; 
        }
        printf("number sent from parent :%d\n" , number) ; 
        write(mypipe[0][1], &number, sizeof(int));
        write(mypipe[1][1], &number, sizeof(int));
        //sleep(1) ; 
    }
    wait(NULL) ; 
    wait(NULL) ; 
    read(mypipe[0][0], &mean , sizeof(int));
    read(mypipe[1][0], &min , sizeof(int));
    printf( "Min : %d  Mean : %d\n" , min , mean/line_num ) ; 

    return 0;

}

    
