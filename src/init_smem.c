#include "smem.h"

int main() {
    // Create shared memory
    int shmid = create_shmem();
    SharedMemory *shm = attach_shmem(shmid);

    // Initialize semaphores
    init_semaphores(shm);

    // Print shared memory ID
    printf("Shared memory ID: %d\n", shmid);

    // Detach shared memory
    detach_shmem(shm);

    return 0;
}
