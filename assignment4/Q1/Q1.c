#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_CUSTOMERS 5 // number of customers
#define NUM_TRANSACTION 6 // number of transactions/threads
#define INIT_STOCK 100 // the initial value for each person stock

// type transaction. you should pass an object of this type to the function arguments while creating thread
typedef struct
{
    double amount;
    int customer_id;
} transaction_t;

double safeBox = 1000.0; // shared variable for all customers

///////// Start your code here ///////////

/*1. define an array to store the stock of all customers
  2. define the necessary mutexes for each customer account and safe box*/
double stocks[NUM_CUSTOMERS] ; 
pthread_mutex_t mutexes[NUM_CUSTOMERS]  ; 
pthread_t threads[NUM_TRANSACTION];
transaction_t transactions[NUM_TRANSACTION] ; 
pthread_mutex_t lock;


void* withdraw(void* arg){
      transaction_t* transaction = (transaction_t*)arg ; 
      int id = transaction->customer_id-1; 
      pthread_mutex_lock(&mutexes[id]); 
      double amount = transaction->amount ; 
      double pre_amount = stocks[id] ; 
      if(amount >= 0 )
      {  
        stocks[id] -= amount ; 
        double taken_amount_safebox = 0 ; 
        if( stocks[id] < 0 )
        { 
            if( pre_amount <= 0 )
                taken_amount_safebox = amount ; 
            else
                taken_amount_safebox = (-stocks[id]); 

            while ( safeBox <= 0 || taken_amount_safebox > safeBox )
            {
                printf("customer %d  \n ERROR occured while withdrawing safebox doesn't have enough amount.wait !!" , id +1) ; 
            }   
            pthread_mutex_lock(&lock) ; 
                safeBox -=taken_amount_safebox ; 
            pthread_mutex_unlock(&lock) ;   
        }
        printf("Customer %d Previouse amount :%f withdrew %f and %f from safe-box. New balance: %f\tsafe-box value: %f\n" , id+1 ,pre_amount, amount , taken_amount_safebox , stocks[id] , safeBox );
        
      }
      else{
        printf("amount should be greater than 0 . customer %d" , id +1) ; }
    pthread_mutex_unlock(&mutexes[id]); 
    return transaction ; 

}

void* deposit(void* arg)
{
    transaction_t* transaction = (transaction_t*)arg ; 
    int id = transaction->customer_id -1 ; 
    pthread_mutex_lock(&mutexes[id]); 
    double amount = transaction->amount ; 
    double pre_amount = stocks[id] ; 
    if( amount >= 0 )
    {
        double taken_amount_safebox =0 ;
        double min = 0 ; 
        if (stocks[id] < 0 )
        {     
            taken_amount_safebox = (-stocks[id]) ; 
            min = (amount < taken_amount_safebox ) ? amount : taken_amount_safebox;
            pthread_mutex_lock(&lock) ; 
            safeBox += min ;      
            pthread_mutex_unlock(&lock) ;
        }
        stocks[id] += amount ; 
        printf("Customer %d Previous amount : %f stock was charged by %f. New balance: %f\tsafe-box value: %f\n" , id+1,pre_amount, amount ,stocks[id] , safeBox );
    }
    else{
        printf("amount should be greater than 0 . customer %d" , id +1) ; 
    }
    pthread_mutex_unlock(&mutexes[id]); 
    return transaction ;
}


int generateRandomInteger(int min , int max) {
    return min + rand() % (max - min + 1);
}

double generateRandomDouble(double min, double max) {
    double range = max - min;
    double scaled = (double)rand() / RAND_MAX;
    return min + (scaled * range);
}

int main(){
    srand(time(NULL));

    /* for each customer, value the stock with INIT_STOCK
    init mutex for each account and safe box */
   
    for(int i=0 ; i< NUM_CUSTOMERS ; i++ )
    {
        stocks[i] = INIT_STOCK ; 
        if (pthread_mutex_init(&mutexes[i], NULL) != 0){
            fprintf(stderr, "Mutex initialization failed\n");
            return 1;
        } 
        
    }
    //************************Creating Mutex for SafeBox************************
    pthread_mutex_init(&lock , NULL) ; 
   
    /* create thread for each transaction*/
    for (int i = 0; i < NUM_TRANSACTION; ++i) {
        transactions[i].amount = generateRandomDouble(100 , 600 ) ; 
        transactions[i].customer_id = generateRandomInteger(1 ,5 ) ;
        if( rand() % 2 == 0) 
        {
            if (pthread_create(&threads[i], NULL, withdraw ,(void*)&transactions[i]) != 0) {
                fprintf(stderr, "Thread creation failed\n");
                return 1;
            }
        }
        else
        {
            if (pthread_create(&threads[i], NULL, deposit , (void*)&transactions[i]) != 0) {
                fprintf(stderr, "Thread creation failed\n");
                return 1;
            }
        }
    }

    // Join threads
    for (int i = 0; i < NUM_TRANSACTION ; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Thread join failed\n");
            return 1;
        }
    }
    
    for (int i = 0; i <= NUM_CUSTOMERS; ++i) {
        pthread_mutex_destroy(&mutexes[i]);
    }

    ///////// End of your code ///////////
    return 0;
}