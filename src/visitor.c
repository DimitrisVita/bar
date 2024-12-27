#include "common_types.h"
#include "smem_utils.h"

// Search for an empty chair
void find_chair(SharedMemory* shm) {
    sem_wait(&shm->sit); // Wait for an available chair
    sem_wait(&shm->mutex);

    for (int i = 0; i < NUM_TABLES; i++) {
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            if (shm->tables[i].chairs[j].status == EMPTY) {
                shm->tables[i].chairs[j].pid = getpid();
                shm->tables[i].chairs[j].status = EATING;
                break;
            }
        }
    }

    sem_post(&shm->mutex);
}

int main(int argc, char *argv[]) {
    int opt;
    int resttime = 0;  // Maximum time to rest at the table
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
    SharedMemory* shm = attach_shmem(shmid);

    // Search for an empty chair
    find_chair(shm);

    printf("Visitor %d is eating...\n", getpid());

    // Go to the bar and order
    sem_post(&shm->wakeup);

    // Detach from shared memory
    detach_shmem(shm);

    return 0;
}