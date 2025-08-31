#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int data = 0; // Shared resource
int read_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;          // For writers & first reader
pthread_mutex_t read_count_mutex = PTHREAD_MUTEX_INITIALIZER; // Protect read_count

int read_loops, write_loops;

void* reader(void* arg) {
    int id = *(int*)arg;
    free(arg); // free memory for thread ID

    for (int i = 0; i < read_loops; i++) {
        pthread_mutex_lock(&read_count_mutex);
        read_count++;
        if (read_count == 1) {
            pthread_mutex_lock(&mutex); // first reader locks writers
        }
        pthread_mutex_unlock(&read_count_mutex);

        // Reading
        printf("Reader %d: read data = %d\n", id, data);
        // sleep(1); // optional for lab demo

        pthread_mutex_lock(&read_count_mutex);
        read_count--;
        if (read_count == 0) {
            pthread_mutex_unlock(&mutex); // last reader unlocks writers
        }
        pthread_mutex_unlock(&read_count_mutex);
    }

    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    free(arg); // free memory for thread ID

    for (int i = 0; i < write_loops; i++) {
        pthread_mutex_lock(&mutex);

        data++;
        printf("Writer %d: wrote data = %d\n", id, data);
        // sleep(1); // optional for lab demo

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    int num_readers, num_writers;

    printf("Enter number of reader threads: ");
    scanf("%d", &num_readers);
    printf("Enter number of writer threads: ");
    scanf("%d", &num_writers);

    printf("Enter number of read operations per reader: ");
    scanf("%d", &read_loops);
    printf("Enter number of write operations per writer: ");
    scanf("%d", &write_loops);

    pthread_t readers[num_readers], writers[num_writers];

    // Create reader threads
    for (int i = 0; i < num_readers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&readers[i], NULL, reader, id);
    }

    // Create writer threads
    for (int i = 0; i < num_writers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&writers[i], NULL, writer, id);
    }

    // Join all threads
    for (int i = 0; i < num_readers; i++)
        pthread_join(readers[i], NULL);

    for (int i = 0; i < num_writers; i++)
        pthread_join(writers[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&read_count_mutex);

    printf("\nAll reading and writing operations are done. Final data = %d\n", data);

    return 0;
}