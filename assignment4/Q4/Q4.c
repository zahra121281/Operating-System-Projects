#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <openssl/md5.h>
#define NUMBER_OF_RESOURCES 5
#define NUMBER_OF_THREADS 10
#define MAX_NUMBER_OF_TASKS 3


typedef struct {
    int resources[NUMBER_OF_RESOURCES]; // save the id of thread which resource is belong to 
    sem_t availableResources;
    pthread_mutex_t poolMutex;
} ResourceManager;


typedef struct  
{
    int *times; 
    int num ; 
    int th_id ; 
} ThreadTasks;

ResourceManager manager ;
pthread_t threads[NUMBER_OF_THREADS] ; 
ThreadTasks tasks_pool[MAX_NUMBER_OF_TASKS*NUMBER_OF_THREADS] ; 

void* RunThreadsTasks(void* argv )
{
    int r_id = -1 ; 
    ThreadTasks* tasks = (ThreadTasks*)argv ; 
    int true = 1 ;
    for(int i=0 ; i< tasks->num ; i++ )
    {
        sem_wait(&manager.availableResources) ; 
        while(true)
        {
            pthread_mutex_lock(&manager.poolMutex ) ;  
            for(int j=0 ;j< NUMBER_OF_RESOURCES  ; j++ )
            {
                if( manager.resources[j] == -1 ) 
                {
                    r_id = j ;  
                    manager.resources[r_id] = tasks->th_id ; 
                    true = 0 ; 
                    break; 
                } 
            }
            pthread_mutex_unlock(&manager.poolMutex ) ; 
        }
        printf("Thread %d is performing work with resource %d task number %d/%d\n" , tasks->th_id ,r_id , i+1 , tasks->num) ; 
        sleep(tasks->times[i]) ; 
        pthread_mutex_lock(&manager.poolMutex ) ;  
        manager.resources[r_id] = -1; 
        pthread_mutex_unlock(&manager.poolMutex ) ; 
        sem_post(&manager.availableResources) ; 
    }
    return argv ; 
}


int generateRandomInteger(int min , int max) {
    return min + rand() % (max - min + 1);
}

int main(int argc , char** argv )
{
    // init resource manager :
    for(int i=0 ; i< NUMBER_OF_RESOURCES ; i++ )
        manager.resources[i] = -1 ; 
    sem_init(&manager.availableResources , 0 , NUMBER_OF_RESOURCES ) ; 
    pthread_mutex_init(&manager.poolMutex , NULL ) ; 
    
    int last_t = 0 ; 

    for(int j=0 ; j< NUMBER_OF_THREADS ; j ++ )
    { 
        tasks_pool[last_t].num = generateRandomInteger(1 , MAX_NUMBER_OF_TASKS ) ; 
        tasks_pool[last_t].th_id = j ; 
        tasks_pool[last_t].times = (int*) malloc(sizeof(int)*tasks_pool[last_t].num ) ; 
        for(int k=0 ; k < tasks_pool[last_t].num ; k++)
            tasks_pool[last_t].times[k] = generateRandomInteger(1 , 3) ; 
        pthread_create(&threads[j] ,NULL, RunThreadsTasks , (void*)&tasks_pool[last_t] ) ;
        last_t+=1 ; 
    }

    for (int t = 0; t <NUMBER_OF_THREADS ; ++t) {
        if (pthread_join(threads[t], NULL) != 0) {
            fprintf(stderr, "Thread join failed\n");
            return 1;
        }
    }

    sem_destroy(&manager.availableResources) ; 
    pthread_mutex_destroy(&manager.poolMutex) ; 
    return 0 ; 
}