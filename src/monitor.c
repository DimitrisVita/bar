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

    sem_wait(&shared_memory->mutex);

    printf("Current status of tables:\n");
    for (int i = 0; i < NUM_TABLES; i++) {
        printf("Table %d: ", i);
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            if (shared_memory->tables[i].chairs[j].pid != 0) {
                printf("Chair %d (PID: %d) ", j, shared_memory->tables[i].chairs[j].pid);
            } else {
                printf("Chair %d (empty) ", j);
            }
        }
        printf("\n");
    }

    printf("Total visitors: %d\n", shared_memory->visitorsCount);
    printf("Water count: %d\n", shared_memory->waterCount);
    printf("Cheese count: %d\n", shared_memory->cheeseCount);
    printf("Wine count: %d\n", shared_memory->wineCount);
    printf("Salad count: %d\n", shared_memory->saladCount);
    printf("Total visit duration: %.2f\n", shared_memory->visitsDuration);
    printf("Total waiting duration: %.2f\n", shared_memory->waitingDuration);

    sem_post(&shared_memory->mutex);

    // Detach from shared memory
    detach_shmem(shared_memory);

    return 0;
}
