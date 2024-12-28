#include "common_types.h"
#include "smem_utils.h"

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
    SharedMemory* shm = attach_shmem(shmid);

    // Process orders
    srand(time(NULL)); // Seed the random number generator
    
    while (true) {
        // Wait for a visitor to arrive
        sem_wait(&shm->wakeup);
        
        // ORDERING

        // Randomly decide what to order
        srand(time(NULL));
        int orders[4] = {rand() % 2, rand() % 2, rand() % 2, rand() % 2};

        // Ensure at least one drink is ordered
        if (!orders[0] && !orders[1]) {
            orders[rand() % 2] = 1;
        }

        // Update statistics
        sem_wait(&shm->mutex);
        shm->waterCount += orders[0];
        shm->wineCount += orders[1];
        shm->cheeseCount += orders[2];
        shm->saladCount += orders[3];
        sem_post(&shm->mutex);

        // Wait for the order to be processed

        int min_order_time = (int)(0.50 * ordertime);
        int order_time = min_order_time + rand() % (ordertime - min_order_time + 1);
        printf("Receptionist is processing the order for %d seconds\n", order_time);
        sleep(order_time);
        printf("Receptionist has processed the order\n");
        
        // Release the order semaphore
        sem_post(&shm->order);

        // END ORDERING
    }

    // Detach from shared memory
    detach_shmem(shm);

    // Destroy shared memory
    destroy_shmem(shmid);

    return 0;
}