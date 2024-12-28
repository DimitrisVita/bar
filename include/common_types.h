#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_TABLES 3
#define CHAIRS_PER_TABLE 4

#define NUM_VISITORS 13

enum DRINK_OPTIONS {
    WATER,
    WINE
};

enum FOOD_OPTIONS {
    CHEESE,
    SALAD
};

typedef struct {
    pid_t pid;
} Chair;

typedef struct {
    int table_id;
    Chair chairs[CHAIRS_PER_TABLE];
} Table;

typedef struct {
    // Tables status
    Table tables[NUM_TABLES];

    // Statistics
    int waterCount;
    int cheeseCount;
    int wineCount;
    int saladCount;
    double visitDuration;
    double waitDuration;
    int visitorsCount;

    // Semaphores
    sem_t mutex;            // Mutex for shared memory
    sem_t sit;              // Semaphore for tables
    sem_t order;            // Semaphore for ordering
    sem_t wakeup;           // Semaphore to wake up receptionist
    sem_t log;              // Semaphore for logging
} SharedMemory;

#endif // COMMON_TYPES_H