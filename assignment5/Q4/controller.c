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
#include <signal.h>


int shm_fd;
int* ptr;
sem_t *sem;
sem_t *empty;
sem_t *full;
int pids[CONSUMERS_NUM] ; 

int main()
{
    sem = sem_open(SEM_NAME , O_CREAT, 0666,1);  
    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    ptr = (int *)mmap(0, SIZE, PROT_READ , MAP_SHARED, shm_fd, 0);
    system("gcc -o consomer consumer.c") ; 
    printf("init_controller\n") ; 
    int cons_nums = 0 ; 
    while (1)
    {
        sem_wait(sem) ; 
        int top = ptr[TOP] ; 
        printf("here2 tpp :%d\n" , top ) ;
        sleep(3) ; 
        if (top < DOWN_LIMIT && cons_nums >= 1 )
        {
            int proc_id = pids[cons_nums] ; 
            printf("killing process with pid :%d " , proc_id) ; 
            sem_post(sem) ; 
            kill(proc_id , SIGKILL) ;   
            sleep(2) ; 
        }   
        else if( top >= UP_LIMIT)
        {
            int pid = fork()  ; 
            if ( pid == 0 )
            {
                char * args[] = { "./consumer" , NULL } ; 
                execv( "./consumer" , args )  ; 
                perror("execv") ; 
                exit(EXIT_FAILURE) ; 
            }
            else
            {
                pids[cons_nums] = pid ; 
                cons_nums++ ; 
                printf("new process has been created with pid :%d " , pid) ; 
                sem_post(sem) ; 
                sleep(2) ; 
            }
        }
        else
        {
            printf("in controller . number of consumers are under controller.... number of consumers right now :%d \n" , cons_nums ) ; 
            sem_post(sem) ; 
        }
    }
    
    return EXIT_SUCCESS ;    
}