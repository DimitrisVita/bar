#include "shared_memory.h"  // Include the header file
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>

int main(int argc, char *argv[]) {
    int opt;
    int shmid = 0;  // Shared memory ID

    while ((opt = getopt(argc, argv, "s:")) != -1) {
        switch (opt) {
            case 's':
                shmid = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -s shmid\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (shmid == 0) {
        fprintf(stderr, "Usage: %s -s shmid\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    SharedMemory *shared_memory = (SharedMemory *)shmat(shmid, NULL, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Display the current state of the bar
    for (int i = 0; i < NUM_TABLES; i++) {
        printf("Table %d: ", i);
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            printf("%d ", shared_memory->table_status[i][j]);
        }
        printf("\n");
    }

    // Detach from shared memory
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}