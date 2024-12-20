#include "common_types.h"
#include "smem_utils.h"

int create_shmem() {
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    return shmid;
}

SharedMemory* attach_shmem(int shmid) {
    SharedMemory* shm = (SharedMemory*) shmat(shmid, NULL, 0);
    if (shm == (void*) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shm;
}

void detach_shmem(SharedMemory* shm) {
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

void destroy_shmem(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
}

void init_semaphores(SharedMemory* shm) {
    if (sem_init(&shm->mutex, 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shm->sit, 1, 0) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shm->order, 1, 0) == -1) {
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

void init_statistics(SharedMemory* shm) {
    shm->waterCount = 0;
    shm->cheeseCount = 0;
    shm->wineCount = 0;
    shm->saladCount = 0;
    shm->visitsDuration = 0;
    shm->waitingDuration = 0;
    shm->visitorsCount = 0;
}

void print_statistics(SharedMemory* shm) {
    printf("Water: %d\n", shm->waterCount);
    printf("Cheese: %d\n", shm->cheeseCount);
    printf("Wine: %d\n", shm->wineCount);
    printf("Salad: %d\n", shm->saladCount);
    printf("Average visit duration: %.2f\n", shm->visitsDuration / shm->visitorsCount);
    printf("Average waiting duration: %.2f\n", shm->waitingDuration / shm->visitorsCount);
}

void init_tables(SharedMemory* shm) {
    for (int i = 0; i < NUM_TABLES; i++) {
        shm->tables[i].table_id = i;
        shm->tables[i].isOccupied = false;
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            shm->tables[i].chairs[j].pid = -1;
            shm->tables[i].chairs[j].status = EMPTY;
        }
    }
}

void print_tables(SharedMemory* shm) {
    for (int i = 0; i < NUM_TABLES; i++) {
        printf("Table %d\n", i);
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            printf("Chair %d: %d\n", j, shm->tables[i].chairs[j].pid);
        }
    }
}
