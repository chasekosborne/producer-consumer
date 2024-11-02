#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/table_shm"
#define TABLE_SIZE 2

sem_t *empty, *full, *mutex;
int *table; // buffer
int read_index = 0;

void *consume(void *arg) {
    while (1) {
        sem_wait(full);
        sem_wait(mutex);

        // critical section
        printf("Consumer consumed item: %d\n", table[read_index]);
        table[read_index] = -1;
        read_index = (read_index + 1) % TABLE_SIZE;

        sem_post(mutex);
        sem_post(empty);

        sleep(2); // consumption time
    }

    return NULL;
}

int main() {
    // shared memory
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Consumer: shm_open failed");
        exit(EXIT_FAILURE);
    }
    table = mmap(0, TABLE_SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (table == MAP_FAILED) {
        perror("Consumer: mmap failed");
        exit(EXIT_FAILURE);
    }

    // semaphores
    empty = sem_open("/empty", 0);
    if (empty == SEM_FAILED) {
        perror("Consumer: sem_open /empty failed");
        exit(EXIT_FAILURE);
    }

    full = sem_open("/full", 0);
    if (full == SEM_FAILED) {
        perror("Consumer: sem_open /full failed");
        exit(EXIT_FAILURE);
    }

    mutex = sem_open("/mutex", 0);
    if (mutex == SEM_FAILED) {
        perror("Consumer: sem_open /mutex failed");
        exit(EXIT_FAILURE);
    }

    pthread_t consumer_thread;
    pthread_create(&consumer_thread, NULL, consume, NULL);

    pthread_join(consumer_thread, NULL);

    // cleanup
    munmap(table, TABLE_SIZE * sizeof(int));
    shm_unlink(SHM_NAME);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);

    return 0;
}

