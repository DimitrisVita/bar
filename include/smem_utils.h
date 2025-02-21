#ifndef SMEM_UTILS_H
#define SMEM_UTILS_H

#include "common_types.h"

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

// Logging functions

// Initialize logging
void init_logging(SharedMemory* shm);

// Log a message with a timestamp
void log_message(SharedMemory* shm, const char* format, ...);

// close log file
void close_log(SharedMemory* shm);

#endif // SMEM_UTILS_H