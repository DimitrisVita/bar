#include "smem.h"

// Create shared memory segment and return its id
int create_shmem() {
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    return shmid;
}

// Attach shared memory segment to the calling process
SharedMemory* attach_shmem(int shmid) {
    SharedMemory *shm = (SharedMemory *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shm;
}

// Detach shared memory segment from the calling process
void detach_shmem(SharedMemory* shm) {
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

// Destroy shared memory segment
void destroy_shmem(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
}

// Initialize semaphores in shared memory
void init_semaphores(SharedMemory* shm) {
    if (sem_init(&shm->mutex, 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shm->order_sem, 1, 0) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_VISITORS; i++) {
        if (sem_init(&shm->waitSemaphores[i], 1, 0) == -1) {
            perror("sem_init");
            exit(EXIT_FAILURE);
        }
        if (sem_init(&shm->orderSemaphores[i], 1, 0) == -1) {
            perror("sem_init");
            exit(EXIT_FAILURE);
        }
    }
    shm->waitStart = 0;
    shm->waitEnd = 0;
    shm->orderStart = 0;
    shm->orderEnd = 0;
    printf("Initialized semaphores and circular buffers\n"); // Debug print
}

// Destroy semaphores in shared memory
void destroy_semaphores(SharedMemory* shm) {
    if (sem_destroy(&shm->mutex) == -1) {
        perror("sem_destroy");
    }
    for (int i = 0; i < MAX_VISITORS; i++) {
        if (sem_destroy(&shm->waitSemaphores[i]) == -1) {
            perror("sem_destroy");
        }
        if (sem_destroy(&shm->orderSemaphores[i]) == -1) {
            perror("sem_destroy");
        }
    }
}

// Initialize statistics
void init_statistics(SharedMemory* shm) {
    shm->waterCount = 0;
    shm->cheeseCount = 0;
    shm->wineCount = 0;
    shm->saladCount = 0;
    shm->visitsDuration = 0;
    shm->waitingDuration = 0;
    shm->visitorsCount = 0;
}

// Print statistics
void print_statistics(SharedMemory* shm) {
    printf("Total visitors: %d\n", shm->visitorsCount);
    printf("Water count: %d\n", shm->waterCount);
    printf("Cheese count: %d\n", shm->cheeseCount);
    printf("Wine count: %d\n", shm->wineCount);
    printf("Salad count: %d\n", shm->saladCount);
    printf("Total visit duration: %.2f\n", shm->visitsDuration);
    printf("Total waiting duration: %.2f\n", shm->waitingDuration);
}

// Initialize tables
void init_tables(SharedMemory* shm) {
    for (int i = 0; i < NUM_TABLES; i++) {
        shm->tables[i].table_id = i;
        shm->tables[i].isOccupied = false;
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            shm->tables[i].chairs[j].pid = 0;
            shm->tables[i].chairs[j].status = EMPTY;
        }
    }
}

// Print tables status
void print_tables(SharedMemory* shm) {
    printf("Current status of tables:\n");
    for (int i = 0; i < NUM_TABLES; i++) {
        printf("Table %d: ", i);
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            if (shm->tables[i].chairs[j].pid != 0) {
                printf("Chair %d (PID: %d) ", j, shm->tables[i].chairs[j].pid);
            } else {
                printf("Chair %d (empty) ", j);
            }
        }
        printf("\n");
    }
    printf("Wait buffer start index: %d, end index: %d\n", shm->waitStart, shm->waitEnd); // Debug print
}