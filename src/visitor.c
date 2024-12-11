#include "smem.h"
#include "common.h"

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
    SharedMemory *shared_memory = attach_shmem(shmid);

    // TEST CODE

    // Place order
    printf("Visitor placing order at table\n");
    sem_post(&shared_memory->order_sem);  // Signal the receptionist

    // Simulate eating/drinking time
    sleep((rand() % (resttime / 2)) + (resttime / 2));

    // Detach from shared memory
    detach_shmem(shared_memory);

    return 0;
}