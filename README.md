### producer-consumer
Project #1 for Operating Systems

## Functions

### Common
- **`sem_open`**: Opens or creates a semaphore for synchronization.
- **`sem_wait`**: Decrements (locks) the semaphore; the calling thread is blocked if the semaphore's value is zero.
- **`sem_post`**: Increments (unlocks) the semaphore and potentially wakes up blocked threads.
- **`shm_open`**: Creates or opens a shared memory object.
- **`mmap`**: Maps the shared memory object into the process's address space.
- **`munmap`**: Unmaps the shared memory from the process's address space.
- **`shm_unlink`**: Removes the shared memory object.

### `producer.c`

#### Functions:
- **`main`**:
  - Initializes shared memory for the table.
  - Initializes semaphores: `empty`, `full`, and `mutex`.
  - Creates and starts the producer thread.
  - Cleans up shared memory and semaphores upon termination.

- **`void *produce(void *arg)`**:
  - Produces items in a loop and places them on the table.
  - Waits on the `empty` semaphore to check for available space.
  - Locks the `mutex` semaphore to ensure exclusive access to shared memory.
  - Places an item on the table and increments the `write_index`.
  - Unlocks the `mutex` and signals the `full` semaphore to indicate that an item is available.

### `consumer.c`

#### Functions:
- **`main`**:
  - Attaches to the shared memory.
  - Opens semaphores: `empty`, `full`, and `mutex`.
  - Creates and starts the consumer thread.
  - Cleans up shared memory and semaphores upon termination.

- **`void *consume(void *arg)`**:
  - Consumes items from the table in a loop.
  - Waits on the `full` semaphore to check for available items.
  - Locks the `mutex` semaphore to ensure exclusive access to shared memory.
  - Checks if the item is valid (`!= -1`) and consumes it.
  - Clears the consumed slot and increments the `read_index`.
  - Unlocks the `mutex` and signals the `empty` semaphore to indicate that a space is available.

## Semaphores Used
- **`/empty`**: Tracks available slots in the table.
- **`/full`**: Tracks the number of items available for consumption.
- **`/mutex`**: Ensures mutual exclusion when accessing shared memory.

## Shared Memory
- **`/table_shm`**: A shared buffer that holds the items produced and consumed.
