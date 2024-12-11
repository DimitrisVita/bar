#include "shared_memory.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

int create_shared_memory() {
    key_t key = ftok("bar", 65);
    int shmid = shmget(key, sizeof(SharedMemory), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    return shmid;
}

SharedMemory* attach_shared_memory(int shmid) {
    SharedMemory* shm = (SharedMemory*)shmat(shmid, NULL, 0);
    if (shm == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    return shm;
}

void detach_shared_memory(SharedMemory* shm) {
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }
}

void destroy_shared_memory(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
}

void initialize_semaphores(SharedMemory* shm) {
    for (int i = 0; i < NUM_TABLES; i++) {
        if (sem_init(&shm->table_semaphores[i], 1, 1) == -1) {
            perror("sem_init");
            exit(1);
        }
    }
    if (sem_init(&shm->waiting_sem, 1, 0) == -1) {
        perror("sem_init");
        exit(1);
    }
    if (sem_init(&shm->order_sem, 1, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
    if (sem_init(&shm->mutex, 1, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
    if (sem_init(&shm->buffer_sem, 1, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
}

void destroy_semaphores(SharedMemory* shm) {
    for (int i = 0; i < NUM_TABLES; i++) {
        if (sem_destroy(&shm->table_semaphores[i]) == -1) {
            perror("sem_destroy");
            exit(1);
        }
    }
    if (sem_destroy(&shm->waiting_sem) == -1) {
        perror("sem_destroy");
        exit(1);
    }
    if (sem_destroy(&shm->order_sem) == -1) {
        perror("sem_destroy");
        exit(1);
    }
    if (sem_destroy(&shm->mutex) == -1) {
        perror("sem_destroy");
        exit(1);
    }
    if (sem_destroy(&shm->buffer_sem) == -1) {
        perror("sem_destroy");
        exit(1);
    }
}