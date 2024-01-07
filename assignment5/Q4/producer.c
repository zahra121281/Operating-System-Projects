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


int shm_fd;
int* ptr;
sem_t *sem;
sem_t *empty;
sem_t *full;


void clean_up()
{
    sem_close(full);
    sem_unlink(SEM_FULL);
    sem_close(empty);
    sem_unlink(SEM_EMPTY);
    sem_close(sem);
    sem_unlink(SEM_NAME);
}


int main()
{
    clean_up() ; 
    sem = sem_open(SEM_NAME , O_CREAT, 0666,1);  
    empty = sem_open(SEM_EMPTY , O_CREAT, 0666, SIZE);
    full = sem_open(SEM_FULL , O_CREAT, 0666,0 );
    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);

    printf("init sem\n");
    if (shm_fd < 0)
    {
        perror("Failed to open shared mem");
        return EXIT_FAILURE;
    }

    ftruncate(shm_fd, SIZE);
    ptr = (int *)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); 
    ptr[TOP] = 0 ; // pointer to the top of the stack ; 
    while(1)
    {
        // generate random number 
        int rand_num = 1 +( rand() % (20 - 1+ 1)) ; 
        sem_wait(empty);
        sem_wait(sem) ; 
        ptr[TOP]+=1 ; 
        int index = ptr[TOP] ; 
        ptr[index] = rand_num ;
        printf("Top :%d Sent!: %d\n",index , rand_num) ;
        //sleep(2);
        sem_post(full);
        sem_post(sem);
    }
    
    return EXIT_SUCCESS;
}


    