#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <semaphore.h>
#include <sys/types.h>
#include "common.h"

typedef struct {
    pid_t pid;
    int status;
} Chair;

typedef struct {
    int table_id;
    Chair chairs[CHAIRS_PER_TABLE];
} Table;

typedef struct {
    // Tables status
    Table tables[NUM_TABLES];

    // Statistics
    int waterCount;
    int cheeseCount;
    int wineCount;
    int saladCount;
    double visitsDuration;
    double waitingDuration;
    int visitorsCount;

    // Semaphores
    sem_t mutex;    // Mutex for accessing shared memory
    sem_t table_sem[NUM_TABLES];    // Semaphore for each table
    sem_t order_sem;    // Semaphore for order processing
    sem_t orderSemaphores[MAX_VISITORS]; // Semaphore for ordering
    sem_t seatSemaphores[MAX_VISITORS];  // Semaphore for selecting seats

    // Circular buffer
    pid_t waitingBuffer[MAX_VISITORS];
    int bufferStart;
    int bufferEnd;

    // Waiting semaphores
    sem_t waitingSemaphores[MAX_VISITORS];
} SharedMemory;


// Shared memory functions

// Create shared memory segment and return its id
int create_shmem();

// Attach shared memory segment to the calling process
SharedMemory* attach_shmem(int shmid);

// Detach shared memory segment from the calling process
void detach_shmem(SharedMemory* shm);

// Destroy shared memory segment
void destroy_shmem(int shmid);

// Semaphore functions

// Initialize semaphores in shared memory
void init_semaphores(SharedMemory* shm);

// Destroy semaphores in shared memory
void destroy_semaphores(SharedMemory* shm);

#endif // SHARED_MEMORY_H