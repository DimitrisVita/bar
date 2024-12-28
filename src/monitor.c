#include "common_types.h"
#include "smem_utils.h"

int main(int argc, char *argv[]) {
    int shmid = 0;
    int opt;

    // Parse command line arguments
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

    // Check if the required arguments are provided
    if (shmid == 0) {
        fprintf(stderr, "Usage: %s -s shmid\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    SharedMemory *shm = attach_shmem(shmid);    // Attach to shared memory

    sem_wait(&shm->mutex);  // Mutex for critical section

    print_statistics(shm);

    print_tables(shm);  // Print tables status

    sem_post(&shm->mutex);  // Release the mutex

    detach_shmem(shm);  // Detach from shared memory

    return 0;
}
