#include "smem.h"
#include "common.h"
#include <stdlib.h>
#include <time.h>

void service_visitor(int ordertime, SharedMemory *shared_memory) {
    // Process the order
    srand(time(NULL));
    int order_time = (rand() % (ordertime / 2 + 1)) + (ordertime / 2);
    printf("Receptionist processing order for visitor for %d seconds\n", order_time);
    sleep(order_time);

    // Signal the visitor that their order is processed
    sem_post(&shared_memory->orderSemaphores[shared_memory->orderStart]);
    shared_memory->orderStart = (shared_memory->orderStart + 1) % MAX_VISITORS;
}

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
    SharedMemory *shared_memory = attach_shmem(shmid);

    // Process orders
    srand(time(NULL)); // Seed the random number generator

    while (1) {
        // Wait for an order to be placed
        sem_wait(&shared_memory->order_sem);

        // Service the visitor
        service_visitor(ordertime, shared_memory);
    }

    // Detach from shared memory
    detach_shmem(shared_memory);

    // Destroy the shared memory segment
    destroy_shmem(shmid);

    return 0;
}
