#include "common_types.h"
#include "smem_utils.h"

int main() {
    int shmid = create_shmem();
    SharedMemory* shm = attach_shmem(shmid);

    // Initialize shared memory
    init_semaphores(shm);
    init_statistics(shm);
    init_tables(shm);
    init_logging(shm);

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
        execlp("./receptionist", "./receptionist", "-d", "3", "-s", shmid_str, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    // Fork and start the visitor processes
    for (int i = 0; i < NUM_VISITORS; i++) {
        pid_t visitor_pid = fork();
        if (visitor_pid == 0) {
            // Seed the random number generator with a unique value
            srand(time(NULL));
            execl("./visitor", "visitor", "-d", "3", "-s", shmid_str, (char *)NULL);
            perror("execl");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all visitor processes to finish
    for (int i = 0; i < NUM_VISITORS; i++) {
        wait(NULL);
    }

    // Press Enter to terminate the receptionist process and end the program
    printf("Press Enter to terminate the receptionist process...\n");
    getchar();

    // Send termination signal to the receptionist process
    kill(receptionist_pid, SIGTERM);

    // Wait for the receptionist process to finish
    waitpid(receptionist_pid, NULL, 0);

    // Print statistics before termination
    sem_wait(&shm->mutex);
    print_statistics(shm);
    sem_post(&shm->mutex);

    close_log(shm);
    detach_shmem(shm);
    destroy_shmem(shmid);

    return 0;
}