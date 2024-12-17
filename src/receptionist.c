#include "smem.h"
#include "common.h"
#include <stdlib.h>
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
    SharedMemory *shared_memory = attach_shmem(shmid);

    // Process orders
    srand(time(NULL)); // Seed the random number generator

    while (1) { // Loop to keep the receptionist running
        // Wait for a client
        sem_wait(&shared_memory->order_sem);

        // Critical section to get the next visitor from the queue
        sem_wait(&shared_memory->mutex);
        pid_t visitor_pid = shared_memory->waitBuffer[shared_memory->waitStart];
        int wait_index = shared_memory->waitStart;
        shared_memory->waitStart = (shared_memory->waitStart + 1) % MAX_VISITORS;
        sem_post(&shared_memory->mutex);

        // Notify the visitor to start ordering
        sem_post(&shared_memory->orderSemaphores[wait_index]);

        // Simulate order processing time
        int sleep_time = (rand() % (ordertime / 2 + 1)) + (ordertime / 2);
        printf("Servicing visitor PID %d for %d seconds\n", visitor_pid, sleep_time);
        sleep(sleep_time);
        printf("Visitor PID %d serviced\n", visitor_pid);	

        // Notify the visitor that their order is ready
        sem_post(&shared_memory->waitSemaphores[wait_index]);
    }

    // Detach from shared memory
    detach_shmem(shared_memory);

    // Destroy the shared memory segment
    destroy_shmem(shmid);

    return 0;
}
