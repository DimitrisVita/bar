#include "common_types.h"
#include "smem_utils.h"

int main(int argc, char *argv[]) {
    int opt;
    int ordertime = 0;  // Maximum time to process an order
    int shmid = 0;      // Shared memory ID

    // Parse command line arguments
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

    // Check if the required arguments are provided
    if (ordertime == 0 || shmid == 0) {
        fprintf(stderr, "Usage: %s -d ordertime -s shmid\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    SharedMemory* shm = attach_shmem(shmid);    // Attach to shared memory

    srand(time(NULL));

    log_message(shm, "Receptionist started");

    while (true) {
        sem_wait(&shm->wakeup); // Wait for a visitor to arrive
        
        // Whats the visitor's order
        srand(time(NULL));
        int orders[4] = {rand() % 2, rand() % 2, rand() % 2, rand() % 2};

        // Ensure at least one drink is ordered
        if (!orders[0] && !orders[1])
            orders[rand() % 2] = 1;

        // Update statistics
        sem_wait(&shm->mutex);
        shm->waterCount += orders[0];
        shm->wineCount += orders[1];
        shm->cheeseCount += orders[2];
        shm->saladCount += orders[3];
        sem_post(&shm->mutex);

        log_message(shm, "Receptionist processing order: Water: %d, Wine: %d, Cheese: %d, Salad: %d", orders[0], orders[1], orders[2], orders[3]);

        // Random order time in the range [0.5 * ordertime, ordertime]
        int min_order_time = (int)(0.50 * ordertime);
        int order_time = min_order_time + rand() % (ordertime - min_order_time + 1);

        sleep(order_time);  // Process the order
        
        sem_post(&shm->order);  // Notify the visitor that the order is ready
    }

    detach_shmem(shm);
    
    return 0;
}
