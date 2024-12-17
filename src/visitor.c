#include "smem.h"
#include "common.h"

void find_empty_chair(SharedMemory* shm) {
    bool found = false;
    for (int i = 0; i < NUM_TABLES; i++) {
        bool table_locked = false;
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            if (shm->tables[i].chairs[j].status == LEAVING) {
                table_locked = true;
                break;
            }
        }
        if (!table_locked && !shm->tables[i].isOccupied) {
            for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
                if (shm->tables[i].chairs[j].status == EMPTY) {
                    // Found an empty chair
                    shm->tables[i].chairs[j].status = EATING;
                    shm->tables[i].chairs[j].pid = getpid();
                    printf("Visitor %d seated at table %d, chair %d\n", getpid(), i, j);
                    found = true;
                    break;
                }
            }
        }
        if (found) break;
    }
    if (!found) {
        // No empty chairs available, add to waiting buffer
        printf("No empty chairs available for visitor %d, waiting...\n", getpid());
        shm->waitBuffer[shm->waitEnd] = getpid();
        int wait_index = shm->waitEnd;
        printf("Visitor %d added to waitBuffer at index %d\n", getpid(), wait_index); // Debug print
        shm->waitEnd = (shm->waitEnd + 1) % MAX_VISITORS;
        sem_post(&shm->mutex);
        sem_wait(&shm->waitSemaphores[wait_index]);
        sem_wait(&shm->mutex);
        find_empty_chair(shm); // Retry finding an empty chair
    }
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
    SharedMemory *shared_memory = attach_shmem(shmid);

    // Find an empty chair of a table
    sem_wait(&shared_memory->mutex);
    find_empty_chair(shared_memory);
    sem_post(&shared_memory->mutex);

    // Notify the receptionist
    sem_wait(&shared_memory->mutex);
    shared_memory->waitBuffer[shared_memory->waitEnd] = getpid();
    int wait_index = shared_memory->waitEnd;
    printf("Visitor %d added to waitBuffer at index %d\n", getpid(), wait_index); // Debug print
    shared_memory->waitEnd = (shared_memory->waitEnd + 1) % MAX_VISITORS;
    sem_post(&shared_memory->mutex);
    sem_post(&shared_memory->order_sem);

    // Wait for the receptionist to allow ordering
    sem_wait(&shared_memory->orderSemaphores[wait_index]);

    // Wait for the order to be processed
    sem_wait(&shared_memory->waitSemaphores[wait_index]);

    // Randomly decide what to order
    int order_water = rand() % 2;
    int order_wine = rand() % 2;
    int order_cheese = rand() % 2;
    int order_salad = rand() % 2;

    // Ensure at least one drink is ordered
    if (!order_water && !order_wine) {
        order_water = 1;
    }

    // Update shared memory with the order
    sem_wait(&shared_memory->mutex);
    if (order_water) shared_memory->waterCount++;
    if (order_wine) shared_memory->wineCount++;
    if (order_cheese) shared_memory->cheeseCount++;
    if (order_salad) shared_memory->saladCount++;
    sem_post(&shared_memory->mutex);

    // Rest at the table
    srand(time(NULL));
    int sleep_time = (rand() % (resttime / 2 + 1)) + (resttime / 2);
    printf("Visitor PID %d resting for %d seconds\n", getpid(), sleep_time);
    sleep(sleep_time);

    // Leave the table
    sem_wait(&shared_memory->mutex);
    for (int i = 0; i < NUM_TABLES; i++) {
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            if (shared_memory->tables[i].chairs[j].pid == getpid()) {
                shared_memory->tables[i].chairs[j].pid = 0;
                shared_memory->tables[i].chairs[j].status = EMPTY;
                bool all_empty = true;
                for (int k = 0; k < CHAIRS_PER_TABLE; k++) {
                    if (shared_memory->tables[i].chairs[k].status != EMPTY) {
                        all_empty = false;
                        break;
                    }
                }
                if (all_empty) {
                    shared_memory->tables[i].isOccupied = false;

                    // Wake up visitors from the waiting buffer (up to 4)
                    int wake_count = 0;
                    while (shared_memory->waitStart != shared_memory->waitEnd && wake_count < 4) {
                        int wait_index = shared_memory->waitStart;
                        printf("Waking up visitor at waitBuffer index %d\n", wait_index); // Debug print
                        shared_memory->waitStart = (shared_memory->waitStart + 1) % MAX_VISITORS;
                        sem_post(&shared_memory->waitSemaphores[wait_index]);
                        wake_count++;
                    }
                }
                break;
            }
        }
    }
    sem_post(&shared_memory->mutex);

    // Update statistics
    sem_wait(&shared_memory->mutex);
    shared_memory->visitorsCount++;
    shared_memory->visitsDuration += sleep_time;
    sem_post(&shared_memory->mutex);

    // Detach from shared memory
    detach_shmem(shared_memory);

    return 0;
}