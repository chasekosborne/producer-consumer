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
int *table;
int write_index = 0;

void *produce(void *arg) {
    int item = 0;

    while (1) {
        sem_wait(empty);
        sem_wait(mutex);

        // critical section
        table[write_index] = item;
        printf("Producer produced item: %d\n", item);
        write_index = (write_index + 1) % TABLE_SIZE;

        sem_post(mutex);
        sem_post(full);
        item++;

        sleep(1); // production time
    }

    return NULL;
}

int main() {
    // shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, TABLE_SIZE * sizeof(int));
    table = mmap(0, TABLE_SIZE * sizeof(int), PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // semaphores
    empty = sem_open("/empty", O_CREAT, 0666, TABLE_SIZE);
    full = sem_open("/full", O_CREAT, 0666, 0);
    mutex = sem_open("/mutex", O_CREAT, 0666, 1); // Initialize mutex with value 1

    pthread_t producer_thread;
    pthread_create(&producer_thread, NULL, produce, NULL);

    pthread_join(producer_thread, NULL);

    // cleanup
    munmap(table, TABLE_SIZE * sizeof(int));
    shm_unlink(SHM_NAME);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);

    return 0;
}

