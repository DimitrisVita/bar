#include "common_types.h"
#include "smem_utils.h"

int main() {
    int shmid = create_shmem();
    SharedMemory* shm = attach_shmem(shmid);

    // Initialize shared memory
    init_semaphores(shm);
    init_statistics(shm);
    init_tables(shm);

    // Print shared memory id
    printf("Shared memory id: %d\n", shmid);

    // Wait until "Enter" is pressed
    printf("Press Enter to continue...\n");
    getchar();

    // Convert shared memory id to string
    char shmid_str[12];
    sprintf(shmid_str, "%d", shmid);

    // Fork and start the receptionist process
    pid_t receptionist_pid = fork();
    if (receptionist_pid == 0) {
        execlp("./receptionist", "./receptionist", "-d", "5", "-s", shmid_str, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    // Fork and start the visitor processes
    for (int i = 0; i < NUM_VISITORS; i++) {
        pid_t visitor_pid = fork();
        if (visitor_pid == 0) {
            execl("./visitor", "visitor", "-d", "1", "-s", shmid_str, (char *)NULL);
            perror("execl");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all visitor processes to finish
    for (int i = 0; i < NUM_VISITORS; i++) {
        wait(NULL);
    }

    destroy_semaphores(shm);
    detach_shmem(shm);
    destroy_shmem(shmid);

    return 0;
}