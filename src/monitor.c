#include "smem.h"
#include "common.h"

int main(int argc, char *argv[]) {
    int shmid = 0;
    int opt;

    while ((opt = getopt(argc, argv, "s:")) != -1) {
        switch (opt) {
            case 's':
                shmid = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -s shmid\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (shmid == 0) {
        fprintf(stderr, "Usage: %s -s shmid\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    SharedMemory *shared_memory = attach_shmem(shmid);

    // Mutex for critical section
    sem_wait(&shared_memory->mutex);

    // Print statistics
    print_statistics(shared_memory);

    // Print tables status
    print_tables(shared_memory);

    // Release mutex
    sem_post(&shared_memory->mutex);

    // Detach from shared memory
    detach_shmem(shared_memory);

    return 0;
}
