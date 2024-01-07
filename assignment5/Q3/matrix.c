#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "shared_mem.h"
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/wait.h>


// result matrix size = 6 x 8 
sem_t *sem ; 
int shm_fd ; 
int* ptr ; 
const int A[ROWS_A][COLS_A] = MATRIX_A;
const int B[COLS_A][COLS_B] = MATRIX_B;


void multiplyer(int arg)
{
    int row = arg ; 
    int mult = 0 ; 
    for (int j = 0; j < COLS_B; ++j) {
        int mult = 0;
        printf("multiplying rows %d of matrix A with Column %d of matrix B\n" , row , j ) ; 
        for (int k = 0; k < COLS_A; ++k) {
            mult += A[row][k] * B[k][j];
        }
        sem_wait(sem ) ; 
        ptr [row*COLS_B +j ]  = mult ; 
        sem_post(sem) ; 
    }

}

int main()
{
    sem = sem_open(SEM_NAME , O_CREAT, 0666, 1);
    printf("init sem\n");
    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0)
    {
        perror("Failed to open shared mem");
        return EXIT_FAILURE;
    }

    ftruncate(shm_fd, SIZE);
    // get a pointer to the first element of sh mem ; 

    ptr = (int *)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    int ids[ROWS_A] ;
    for(int i=0 ; i< ROWS_A ; i++ )
    {
        ids[i] = fork() ; 
        if ( ids[i] == 0 )
        {
            multiplyer(i) ; 
            exit(0)  ; 
        }
        if( ids[i] < 0 ){
            fprintf(stderr, "Process creation failed\n");
            return 1 ; 
        }
        
    }
    
    for(int i=0 ; i < ROWS_A ; i++ )
    {
        wait(NULL) ; 
    }

    printf("print result matrix...\n") ; 
    for(int i=0 ; i < ROWS_A ; i++ )
    {
        for(int j=0 ; j < COLS_B ; j++ )
        {
            printf("%d " , ptr[i*COLS_B+j] ) ; 
        }
        printf("\n") ; 
    }

    munmap(ptr, SIZE);
    close(shm_fd);
    sem_close(sem);
    sem_unlink(SEM_NAME);
    return EXIT_SUCCESS;
}

