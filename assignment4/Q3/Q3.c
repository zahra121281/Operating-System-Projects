#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <openssl/md5.h>
#define BUFFER_SIZE 1024
#define MAX_LENGTH 1024 

sem_t mutex ; 
sem_t rw_mutex ; 
int rw ; 
/*
sem_t sem1 ; 
sem_t sem2 ; 

    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
*/

char line[MAX_LENGTH];

void* producer(void* argv)
{
    FILE *file;
    char* source = (char*)argv ; 
    file = fopen( source , "rb");
    char* l = "init"; 
    if (file == NULL) {
        perror("Error opening file");
        exit(1) ; 
    }

    // Read lines from the file
    while ( 1)
    {
        if( rw == 0 )
        {
            //read from fille
            l =fgets(line, sizeof(line), file) ; 
            sem_post(&mutex) ; 
            if( l == NULL ){
                rw = -1 ; 
                sem_post(&rw_mutex) ; 
                printf("end of producer process\n") ; 
                break; 
            }
            printf("producer line :%s \n" , line ) ; 
            rw+= 1 ; 
            sem_post(&rw_mutex) ; 
        }
    }

    // Close the file
    
    fclose(file);
    return argv ; 
}

void* consumer(void* argv )
{
    FILE *file;
    char* source = (char*)argv ; 
    file = fopen( source , "wb");
    int o = 0 ;
    if (file == NULL) {
        perror("Error opening file\n");
        exit(1) ; 
    }

    // Write liness to file
    while ( 1 )
    {   
        if( rw == 1 )
        {
            sem_wait(&mutex) ; 
            //write to file
            o = fputs(line, file) ;  
            
            printf("consumer line :%s \n" , line ) ; 

            sem_wait(&rw_mutex) ; 
            rw -= 1 ;             
        }
        if( rw == -1 || o == EOF )
        {
            printf("end of consumer process\n") ; 
            break  ; 
        }
    } 
    
    // Close the file
    fclose(file);
    return argv ; 
}



int CompareFilesWithMD5(char* source ,char * dest ) 
{

    return 0 ; 
}

int main(int argc , char** argv)
{
    if ( argc < 3 )
    {
        printf("number of arguments should be 3 : ./copy <source file> <dest file>\n") ; 
        return 0 ; 
    }
    //pthread_mutex_init(&mutex , NULL ) ; 
    sem_init(&mutex, 0,0 );
    sem_init(&rw_mutex, 0,0);
    rw = 0 ; 
    char * source = argv[1] ; 
    char * dest = argv[2] ; 
    pthread_t th[2];
    pthread_create(&th[1], NULL, &producer, (void*)source);
    pthread_create(&th[0], NULL, &consumer, (void*)dest );
    pthread_join( th[1] , NULL ) ;
    pthread_join( th[0] , NULL ) ; 
    
    sem_destroy(&mutex);
    return 0 ; 
}