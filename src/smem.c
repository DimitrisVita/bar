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
    for (int i = 0; i < NUM_TABLES; i++) {
        if (sem_init(&shm->table_sem[i], 1, 1) == -1) {
            perror("sem_init");
            exit(EXIT_FAILURE);
        }
    }
    if (sem_init(&shm->order_sem, 1, 0) == -1) { // Initialize to 0
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_VISITORS; i++) {
        if (sem_init(&shm->waitingSemaphores[i], 1, 0) == -1) {
            perror("sem_init");
            exit(EXIT_FAILURE);
        }
    }
}

// Destroy semaphores in shared memory
void destroy_semaphores(SharedMemory* shm) {
    if (sem_destroy(&shm->mutex) == -1) {
        perror("sem_destroy");
    }
    for (int i = 0; i < NUM_TABLES; i++) {
        if (sem_destroy(&shm->table_sem[i]) == -1) {
            perror("sem_destroy");
        }
    }
    if (sem_destroy(&shm->order_sem) == -1) {
        perror("sem_destroy");
    }
    for (int i = 0; i < MAX_VISITORS; i++) {
        if (sem_destroy(&shm->waitingSemaphores[i]) == -1) {
            perror("sem_destroy");
        }
    }
}
