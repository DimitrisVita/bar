#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int opt;
    int ordertime = 0;  // Maximum time to process an order
    int shmid = 0;      // Shared memory ID

    while ((opt = getopt(argc, argv, "d:s:")) != -1) {
        switch (opt) {
            case 'd':
                ordertime = atoi(optarg);
                break;
            case 's':
                shmid = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -d ordertime -s shmid\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (ordertime == 0 || shmid == 0) {
        fprintf(stderr, "Usage: %s -d ordertime -s shmid\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    void *shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphores and other shared memory structures
    // ...existing code...

    // Main loop to handle receptionist behavior
    while (1) {
        // Wait for a visitor to place an order
        // ...existing code...

        // Simulate order processing time
        int processing_time = (rand() % (ordertime / 2)) + (ordertime / 2);
        sleep(processing_time);

        // Update shared memory with order details
        // ...existing code...

        // Signal that the order is complete
        // ...existing code...
    }

    // Detach from shared memory
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}