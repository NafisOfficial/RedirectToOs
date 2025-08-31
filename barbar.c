#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CHAIRS 3   // Waiting room chairs
#define CUSTOMERS 10

sem_t customers;   // Number of customers waiting
sem_t barber;      // Barber ready
pthread_mutex_t mutex; // For mutual exclusion
int waiting = 0;   // Number of customers waiting

void* barber_function(void* arg) {
    while(1) {
        sem_wait(&customers);  // Wait for a customer
        pthread_mutex_lock(&mutex);
        waiting--;             // Decrease count of waiting customers
        pthread_mutex_unlock(&mutex);

        printf("Barber is cutting hair\n");
        sleep(2);              // Haircut time
        sem_post(&barber);     // Barber is done
    }
}

void* customer_function(void* arg) {
    int id = *((int*)arg);
    pthread_mutex_lock(&mutex);
    if(waiting < CHAIRS) {
        waiting++;
        printf("Customer %d is waiting. Waiting customers: %d\n", id, waiting);
        sem_post(&customers);  // Notify barber
        pthread_mutex_unlock(&mutex);
        sem_wait(&barber);     // Wait for barber to finish
        printf("Customer %d is getting haircut\n", id);
    } else {
        pthread_mutex_unlock(&mutex);
        printf("Customer %d leaves as no chairs available\n", id);
    }
}

int main() {
    pthread_t b, c[CUSTOMERS];
    sem_init(&customers,0,0);
    sem_init(&barber,0,0);
    pthread_mutex_init(&mutex,NULL);

    pthread_create(&b,NULL,barber_function,NULL);

    int ids[CUSTOMERS];
    for(int i=0;i<CUSTOMERS;i++){
        ids[i] = i+1;
        pthread_create(&c[i],NULL,customer_function,&ids[i]);
        sleep(1); // Customers arrive at different times
    }

    for(int i=0;i<CUSTOMERS;i++)
        pthread_join(c[i],NULL);

    pthread_cancel(b);  // Stop barber thread for simplicity
    sem_destroy(&customers);
    sem_destroy(&barber);
    pthread_mutex_destroy(&mutex);

    return 0;
}
