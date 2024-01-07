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


int main()
{
    sleep(1) ; 
    sem = sem_open(SEM_NAME , O_CREAT, 0666,1);  
    empty = sem_open(SEM_EMPTY , O_CREAT, 0666, IN_LEN);
    full = sem_open(SEM_FULL , O_CREAT, 0666,0 );
    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    
    printf("init sem\n");
    if (shm_fd < 0)
    {
        perror("Failed to open shared mem");
        return EXIT_FAILURE;
    }

    ptr = (int *)mmap(0, SIZE, PROT_WRITE , MAP_SHARED, shm_fd, 0);
    printf("init map\n");
    int index ;
    while(1)
    { 
        sem_wait(full) ; 
        sem_wait(sem);
        index = ptr[TOP] ; 
        printf("Top: %d  Recieved: %d\n", index ,ptr[index]);
        ptr[index] = 0 ; 
        sleep(1);
        ptr[0] = index -1 ; 
        sem_post(empty);  
        sem_post(sem);
    }
    return EXIT_SUCCESS ; 
}