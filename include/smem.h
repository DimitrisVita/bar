#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <semaphore.h>
#include <sys/types.h>
#include "common.h"

typedef struct {
    pid_t pid;
    enum { EMPTY, EATING, LEAVING } status;
} Chair;

typedef struct {
    int table_id;
    Chair chairs[CHAIRS_PER_TABLE];
    bool isOccupied;
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
    sem_t mutex;
    sem_t order_sem;

    // Circular buffer for visitors waiting for empty chair
    sem_t waitSemaphores[MAX_VISITORS];
    int waitStart;
    int waitEnd;
    pid_t waitingVisitors[MAX_VISITORS]; // Add this line

    // Circular buffer for visitors waiting for order to be processed
    sem_t orderSemaphores[MAX_VISITORS];
    int orderStart;
    int orderEnd;
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

// Statistics functions

// Initialize statistics
void init_statistics(SharedMemory* shm);

// Print statistics
void print_statistics(SharedMemory* shm);

// Table functions
void init_tables(SharedMemory* shm);

// Print tables
void print_tables(SharedMemory* shm);

#endif // SHARED_MEMORY_H