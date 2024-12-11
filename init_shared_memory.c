#include "shared_memory.h"  // Include the header file
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <unistd.h>

int main() {
    int shmid;
    SharedMemory *shared_data;

    // Create shared memory segment
    shmid = shmget(IPC_PRIVATE, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    shared_data = (SharedMemory *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize shared data
    shared_data->total_visitors = 0;
    shared_data->water_count = 0;
    shared_data->wine_count = 0;
    shared_data->cheese_count = 0;
    shared_data->salad_count = 0;
    shared_data->total_wait_time = 0.0;
    shared_data->total_stay_time = 0.0;

    // Initialize semaphores
    for (int i = 0; i < NUM_TABLES; i++) {
        if (sem_init(&shared_data->table_semaphores[i], 1, 1) == -1) {
            perror("sem_init");
            exit(EXIT_FAILURE);
        }
    }
    if (sem_init(&shared_data->waiting_sem, 1, 0) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shared_data->order_sem, 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shared_data->mutex, 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // Initialize circular buffer semaphore
    if (sem_init(&shared_data->buffer_sem, 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // Initialize visitor PIDs array
    for (int i = 0; i < MAX_VISITORS; i++) {
        shared_data->visitor_pids[i] = 0;
    }

    // Print shared memory ID
    printf("Shared memory ID: %d\n", shmid);

    // Detach from shared memory
    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}