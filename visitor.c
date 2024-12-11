#include "shared_memory.h"  // Include the header file
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int opt;
    int resttime = 0;  // Maximum time to stay at the table
    int shmid = 0;     // Shared memory ID

    while ((opt = getopt(argc, argv, "d:s:")) != -1) {
        switch (opt) {
            case 'd':
                resttime = atoi(optarg);
                break;
            case 's':
                shmid = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -d resttime -s shmid\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (resttime == 0 || shmid == 0) {
        fprintf(stderr, "Usage: %s -d resttime -s shmid\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    SharedMemory *shared_memory = (SharedMemory *)shmat(shmid, NULL, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Visitor behavior
    // Find a table with available chairs
    int table_id = -1;
    for (int i = 0; i < NUM_TABLES; i++) {
        if (sem_wait(&shared_memory->table_semaphores[i]) == 0) {
            table_id = i;
            break;
        }
    }

    if (table_id == -1) {
        printf("No available table. Visitor is waiting.\n");
        sem_wait(&shared_memory->waiting_sem);
        for (int i = 0; i < NUM_TABLES; i++) {
            if (sem_wait(&shared_memory->table_semaphores[i]) == 0) {
                table_id = i;
                break;
            }
        }
    }

    // Place order
    sem_wait(&shared_memory->order_sem);
    // Simulate placing an order
    printf("Visitor placing order at table %d\n", table_id);
    shared_memory->total_visitors++;
    shared_memory->water_count++;  // Example: increment water count
    sem_post(&shared_memory->order_sem);

    // Simulate eating/drinking time
    sleep((rand() % (resttime / 2)) + (resttime / 2));

    // Leave the table
    sem_post(&shared_memory->table_semaphores[table_id]);

    // Detach from shared memory
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}