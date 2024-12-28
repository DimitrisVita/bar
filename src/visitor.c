#include "common_types.h"
#include "smem_utils.h"

// Search for an empty chair
void find_chair(SharedMemory* shm) {
    sem_wait(&shm->sit); // Wait for an available chair
    sem_wait(&shm->mutex);

    bool found = false;
    for (int i = 0; i < NUM_TABLES && !found; i++) {
        for (int j = 0; j < CHAIRS_PER_TABLE && !found; j++) {
            if (shm->tables[i].chairs[j].pid == 0) { // Check if the chair is empty
                shm->tables[i].chairs[j].pid = getpid();
                found = true;
            }
        }
    }

    sem_post(&shm->mutex);
}

// Leave the chair
void leave_chair(SharedMemory* shm) {
    sem_wait(&shm->mutex);

    for (int i = 0; i < NUM_TABLES; i++) {
        bool all_left = true;
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            if (shm->tables[i].chairs[j].pid == getpid()) {
                shm->tables[i].chairs[j].pid = -1; // Mark the chair as left
            }
            if (shm->tables[i].chairs[j].pid != -1) {
                all_left = false;
            }
        }
        if (all_left) {
            // Initialize the table
            for (int j = 0; j < CHAIRS_PER_TABLE; j++)
                shm->tables[i].chairs[j].pid = 0;

            // Increment the semaphore to signal 4 chairs are available
            for (int j = 0; j < CHAIRS_PER_TABLE; j++)
                sem_post(&shm->sit);
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

    sem_post(&shm->wakeup);

    sem_wait(&shm->order);

    // Rest at the table
    srand(time(NULL));
    int min_rest_time = (int)(0.70 * resttime);
    int rest_time = min_rest_time + rand() % (resttime - min_rest_time + 1);
    sleep(rest_time);

    // Leave the table
    leave_chair(shm);

    // Detach from shared memory
    detach_shmem(shm);

    return 0;
}