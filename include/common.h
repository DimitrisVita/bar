#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>


#define NUM_TABLES 3
#define CHAIRS_PER_TABLE 4
#define MAX_VISITORS 100

// Enum for drink options
enum DRINK_OPTIONS {
    WATER,
    WINE
};

// Enum for food options
enum FOOD_OPTIONS {
    CHEESE,
    SALAD
};

#endif // COMMON_H