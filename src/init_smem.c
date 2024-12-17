#include "smem.h"
#include <unistd.h>
#include <sys/wait.h>

#define NUM_VISITORS 100

int main() {
    // Create shared memory
    int shmid = create_shmem();
    SharedMemory *shm = attach_shmem(shmid);

    // Initialize semaphores
    init_semaphores(shm);

    // Initialize statistics
    init_statistics(shm);

    // Initialize tables
    init_tables(shm);

    // Print shared memory ID
    printf("Shared memory ID: %d\n", shmid);

    // Convert shared memory ID to string
    char shmid_str[10];
    sprintf(shmid_str, "%d", shmid);

    // Fork and start the receptionist process
    pid_t receptionist_pid = fork();
    if (receptionist_pid == 0) {
        execl("./receptionist", "receptionist", "-d", "2", "-s", shmid_str, (char *)NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // Fork and start the visitor processes
    for (int i = 0; i < NUM_VISITORS; i++) {
        pid_t visitor_pid = fork();
        if (visitor_pid == 0) {
            execl("./visitor", "visitor", "-d", "2", "-s", shmid_str, (char *)NULL);
            perror("execl");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all visitor processes to finish
    for (int i = 0; i < NUM_VISITORS; i++) {
        wait(NULL);
    }

    // Detach shared memory
    detach_shmem(shm);

    return 0;
}
