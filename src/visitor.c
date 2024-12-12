#include "smem.h"
#include "common.h"

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

    // Select a table
    int table_id = -1;
    while (table_id == -1) {
        for (int i = 0; i < NUM_TABLES; i++) {
            if (sem_trywait(&shared_memory->table_sem[i]) == 0) {
                table_id = i;
                break;
            }
        }
        if (table_id == -1) {
            sleep(1); // Wait for a while before trying again
        }
    }

    // Sit at the table
    sem_wait(&shared_memory->mutex);
    for (int i = 0; i < CHAIRS_PER_TABLE; i++) {
        if (shared_memory->tables[table_id].chairs[i].pid == 0) {
            shared_memory->tables[table_id].chairs[i].pid = getpid();
            break;
        }
    }
    sem_post(&shared_memory->mutex);

    // Notify the receptionist
    sem_wait(&shared_memory->mutex);
    shared_memory->waitingBuffer[shared_memory->bufferEnd] = getpid();
    shared_memory->bufferEnd = (shared_memory->bufferEnd + 1) % MAX_VISITORS;
    sem_post(&shared_memory->mutex);
    sem_post(&shared_memory->order_sem);

    // Wait for the receptionist to allow ordering
    sem_wait(&shared_memory->orderSemaphores[getpid() % MAX_VISITORS]);

    // Notify the receptionist that seat selection is done
    sem_post(&shared_memory->seatSemaphores[getpid() % MAX_VISITORS]);

    // Wait for the order to be processed
    sem_wait(&shared_memory->waitingSemaphores[getpid() % MAX_VISITORS]);

    // Rest at the table
    srand(time(NULL));
    int sleep_time = (rand() % (resttime / 2 + 1)) + (resttime / 2);
    printf("Visitor PID %d resting for %d seconds\n", getpid(), sleep_time);
    sleep(sleep_time);

    // Leave the table
    sem_wait(&shared_memory->mutex);
    for (int i = 0; i < CHAIRS_PER_TABLE; i++) {
        if (shared_memory->tables[table_id].chairs[i].pid == getpid()) {
            shared_memory->tables[table_id].chairs[i].pid = 0;
            break;
        }
    }
    sem_post(&shared_memory->mutex);
    sem_post(&shared_memory->table_sem[table_id]);

    // Update statistics
    sem_wait(&shared_memory->mutex);
    shared_memory->visitorsCount++;
    shared_memory->visitsDuration += sleep_time;
    sem_post(&shared_memory->mutex);

    // Detach from shared memory
    detach_shmem(shared_memory);

    return 0;
}