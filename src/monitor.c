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

    // Local copies of shared memory data
    SharedMemory local_copy;

    // Mutex for critical section
    sem_wait(&shared_memory->mutex);

    // Copy data from shared memory
    local_copy = *shared_memory;

    // Release mutex
    sem_post(&shared_memory->mutex);

    // Print statistics
    print_statistics(&local_copy);

    // Print tables status
    print_tables(&local_copy);

    // Detach from shared memory
    detach_shmem(shared_memory);

    return 0;
}
