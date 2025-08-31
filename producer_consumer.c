#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int *buffer;
int in = 0, out = 0;
int SIZE;       // Buffer size
int total_items;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < total_items; i++) {
        int item = id * 10 + i + 1; // deterministic item

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("[Producer %d] Produced: %d\n", id, item);
        in = (in + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    free(arg);  // Free allocated memory for thread id
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < total_items; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("           [Consumer %d] Consumed: %d\n", id, item);
        out = (out + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    free(arg);  // Free allocated memory for thread id
    return NULL;
}

int main() {
    int num_producers, num_consumers;

    // User inputs
    printf("Enter buffer size: ");
    scanf("%d", &SIZE);
    if(SIZE <= 0){
        printf("Buffer size must be positive!\n");
        return 1;
    }

    printf("Enter number of producers: ");
    scanf("%d", &num_producers);
    printf("Enter number of consumers: ");
    scanf("%d", &num_consumers);
    printf("Enter number of items per producer/consumer: ");
    scanf("%d", &total_items);

    if(num_producers <= 0 || num_consumers <= 0 || total_items <= 0){
        printf("Producers, consumers and items must be positive!\n");
        return 1;
    }

    buffer = (int*)malloc(SIZE * sizeof(int));

    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t prod_threads[num_producers], cons_threads[num_consumers];

    // Create producer threads
    for (int i = 0; i < num_producers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&prod_threads[i], NULL, producer, id);
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&cons_threads[i], NULL, consumer, id);
    }

    // Join producer threads
    for (int i = 0; i < num_producers; i++)
        pthread_join(prod_threads[i], NULL);

    // Join consumer threads
    for (int i = 0; i < num_consumers; i++)
        pthread_join(cons_threads[i], NULL);

    // Final message
    printf("\nAll items have been consumed. Buffer is now empty.\n");

    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    free(buffer);

    return 0;
}