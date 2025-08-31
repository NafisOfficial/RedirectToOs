#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PHILOSOPHERS 10

pthread_mutex_t chopsticks[MAX_PHILOSOPHERS];

void* philosopher(void* arg) {
    int id = *(int*)arg;
    free(arg); // free allocated memory

    printf("Philosopher %d is thinking...\n", id);
    sleep(1); // thinking

    // Deadlock-free chopstick picking
    if (id == MAX_PHILOSOPHERS - 1) {
        // Last philosopher picks right then left
        pthread_mutex_lock(&chopsticks[(id + 1) % MAX_PHILOSOPHERS]);
        printf("Philosopher %d picked up right chopstick %d\n", id, (id + 1) % MAX_PHILOSOPHERS);

        pthread_mutex_lock(&chopsticks[id]);
        printf("Philosopher %d picked up left chopstick %d\n", id, id);
    } else {
        // Others pick left then right
        pthread_mutex_lock(&chopsticks[id]);
        printf("Philosopher %d picked up left chopstick %d\n", id, id);

        pthread_mutex_lock(&chopsticks[(id + 1) % MAX_PHILOSOPHERS]);
        printf("Philosopher %d picked up right chopstick %d\n", id, (id + 1) % MAX_PHILOSOPHERS);
    }

    // Eating
    printf("Philosopher %d is eating...\n", id);
    sleep(2);

    // Put down chopsticks
    pthread_mutex_unlock(&chopsticks[id]);
    pthread_mutex_unlock(&chopsticks[(id + 1) % MAX_PHILOSOPHERS]);

    printf("Philosopher %d finished eating and put down chopsticks.\n\n", id);

    pthread_exit(NULL);
}

int main() {
    int NUM;

    printf("Enter the number of philosophers (2-10): ");
    scanf("%d", &NUM);

    if (NUM < 2 || NUM > MAX_PHILOSOPHERS) {
        printf("Invalid number of philosophers. Enter 2-%d\n", MAX_PHILOSOPHERS);
        return -1;
    }

    pthread_t philosophers[NUM];

    // Initialize chopsticks
    for (int i = 0; i < NUM; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&philosophers[i], NULL, philosopher, id);
    }

    // Join all threads
    for (int i = 0; i < NUM; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy chopsticks
    for (int i = 0; i < NUM; i++) {
        pthread_mutex_destroy(&chopsticks[i]);
    }

    printf("All philosophers have finished eating.\n");

    return 0;
}