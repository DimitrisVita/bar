#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <unistd.h>
#include <getopt.h>


#define NUM_TABLES 3
#define CHAIRS_PER_TABLE 4
#define MAX_VISITORS 100