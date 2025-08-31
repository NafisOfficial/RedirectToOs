#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int *buffer;
int *input_buffer;
int buffer_size;
int num_producers;
int num_consumers;
int total_items;

int in = 0;
int out = 0;
int produced_count = 0;
int consumed_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

// Producer function
void* producer(void* arg) {
    int producer_id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (produced_count >= total_items) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Wait if buffer is full
        while ((in + 1) % buffer_size == out) {
            pthread_cond_wait(&empty, &mutex);
        }

        if (produced_count >= total_items) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        int item = input_buffer[produced_count];
        buffer[in] = item;
        printf("Producer %d produced item: %d\n", producer_id, item);
        in = (in + 1) % buffer_size;
        produced_count++;

        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
    }

    free(arg);
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    int consumer_id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (consumed_count >= total_items) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Wait if buffer is empty
        while (in == out) {
            pthread_cond_wait(&full, &mutex);
        }

        if (consumed_count >= total_items) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        int item = buffer[out];
        printf("Consumer %d consumed item: %d\n", consumer_id, item);
        out = (out + 1) % buffer_size;
        consumed_count++;

        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }

    free(arg);
    return NULL;
}

int main() {
    printf("Enter buffer size: ");
    scanf("%d", &buffer_size);
    printf("Enter number of producers: ");
    scanf("%d", &num_producers);
    printf("Enter number of consumers: ");
    scanf("%d", &num_consumers);
    printf("Enter total number of items to produce: ");
    scanf("%d", &total_items);

    if(buffer_size <= 0 || num_producers <= 0 || num_consumers <= 0 || total_items <= 0) {
        printf("All inputs must be positive numbers.\n");
        return 1;
    }

    buffer = (int*)malloc(buffer_size * sizeof(int));
    input_buffer = (int*)malloc(total_items * sizeof(int));

    printf("Enter %d items to produce:\n", total_items);
    for (int i = 0; i < total_items; i++) {
        printf("Item %d: ", i + 1);
        scanf("%d", &input_buffer[i]);
    }

    pthread_t producers[num_producers], consumers[num_consumers];

    // Create producer threads
    for (int i = 0; i < num_producers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&producers[i], NULL, producer, id);
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&consumers[i], NULL, consumer, id);
    }

    // Join threads
    for (int i = 0; i < num_producers; i++)
        pthread_join(producers[i], NULL);

    for (int i = 0; i < num_consumers; i++)
        pthread_join(consumers[i], NULL);

    printf("\nAll items consumed. Buffer is now empty.\n");

    free(buffer);
    free(input_buffer);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);

    return 0;
}