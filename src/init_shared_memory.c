#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <unistd.h>

int main() {
    int shmid;
    shared_data_t *shared_data;

    // Create shared memory segment
    shmid = shmget(IPC_PRIVATE, sizeof(shared_data_t), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    shared_data = (shared_data_t *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize shared data
    shared_data->visitor_count = 0;
    shared_data->water_count = 0;
    shared_data->wine_count = 0;
    shared_data->cheese_count = 0;
    shared_data->salad_count = 0;
    shared_data->total_wait_time = 0.0;
    shared_data->total_stay_time = 0.0;

    // Initialize semaphores
    for (int i = 0; i < NUM_TABLES; i++) {
        if (sem_init(&shared_data->table_sem[i], 1, CHAIRS_PER_TABLE) == -1) {
            perror("sem_init");
            exit(EXIT_FAILURE);
        }
    }
    if (sem_init(&shared_data->order_sem, 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shared_data->mutex, 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
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