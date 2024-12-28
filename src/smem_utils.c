#include "common_types.h"
#include "smem_utils.h"

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
    SharedMemory* shm = (SharedMemory*) shmat(shmid, NULL, 0);
    if (shm == (void*) -1) {
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
    if (sem_init(&shm->sit, 1, NUM_TABLES * CHAIRS_PER_TABLE) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shm->order, 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shm->wakeup, 1, 0) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shm->log, 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
}

// Destroy semaphores in shared memory
void destroy_semaphores(SharedMemory* shm) {
    if (sem_destroy(&shm->mutex) == -1) {
        perror("sem_destroy");
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(&shm->sit) == -1) {
        perror("sem_destroy");
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(&shm->order) == -1) {
        perror("sem_destroy");
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(&shm->wakeup) == -1) {
        perror("sem_destroy");
        exit(EXIT_FAILURE);
    }
    if (sem_destroy(&shm->log) == -1) {
        perror("sem_destroy");
        exit(EXIT_FAILURE);
    }
}

// Initialize statistics
void init_statistics(SharedMemory* shm) {
    shm->waterCount = 0;
    shm->cheeseCount = 0;
    shm->wineCount = 0;
    shm->saladCount = 0;
    shm->visitDuration = 0;
    shm->waitDuration = 0;
    shm->visitorsCount = 0;
}

// Print statistics
void print_statistics(SharedMemory* shm) {
    printf("Water: %d\n", shm->waterCount);
    printf("Wine: %d\n", shm->wineCount);
    printf("Cheese: %d\n", shm->cheeseCount);
    printf("Salad: %d\n", shm->saladCount);
    double avgVisitDur = shm->visitorsCount > 0 ? shm->visitDuration / shm->visitorsCount : 0;
    printf("Average visit duration: %f\n", avgVisitDur);
    double avgWaitDur = shm->visitorsCount > 0 ? shm->waitDuration / shm->visitorsCount : 0;
    printf("Average wait duration: %f\n", avgWaitDur);
    printf("Visitors count: %d\n", shm->visitorsCount);
}

// Initialize tables
void init_tables(SharedMemory* shm) {
    for (int i = 0; i < NUM_TABLES; i++) {
        shm->tables[i].table_id = i;
        for (int j = 0; j < CHAIRS_PER_TABLE; j++)
            shm->tables[i].chairs[j].pid = 0;
    }
}

// Print tables
void print_tables(SharedMemory* shm) {
    for (int i = 0; i < NUM_TABLES; i++) {
        printf("Table %d\n", i);
        for (int j = 0; j < CHAIRS_PER_TABLE; j++)
            printf("\tChair %d: %d\n", j, shm->tables[i].chairs[j].pid);
    }
}
