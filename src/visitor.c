#include "common_types.h"
#include "smem_utils.h"
#include <sys/times.h>

// Search for an empty chair
void find_chair(SharedMemory* shm) {
    // Start timer for how long the visitor waits to find a chair
    struct tms tb1, tb2;
    double t1, t2, ticspersec;
    ticspersec = (double) sysconf(_SC_CLK_TCK);
    t1 = (double) times(&tb1);

    sem_wait(&shm->sit);    // Wait for an available chair

    // Stop timer for how long the visitor waits to find a chair
    t2 = (double) times(&tb2);
    double waiting_duration = (t2 - t1) / ticspersec;

    // Find the first empty chair
    sem_wait(&shm->mutex);
    
    bool found = false;
    for (int i = 0; i < NUM_TABLES && !found; i++) {
        for (int j = 0; j < CHAIRS_PER_TABLE && !found; j++) {
            if (shm->tables[i].chairs[j].pid == 0) {    // Found an empty chair
                shm->tables[i].chairs[j].pid = getpid();    // Occupy the chair
                found = true;
            }
        }
    }

    shm->visitorsCount++;   // Increment the visitors count
    shm->waitDuration += waiting_duration;    // Update the total waiting duration

    sem_post(&shm->mutex);
}

// Leave the chair
void leave_chair(SharedMemory* shm) {
    sem_wait(&shm->mutex);

    // Update the chair status
    for (int i = 0; i < NUM_TABLES; i++) {
        bool all_left = true;
        for (int j = 0; j < CHAIRS_PER_TABLE; j++) {
            if (shm->tables[i].chairs[j].pid == getpid())       // Leave the chair
                shm->tables[i].chairs[j].pid = -1;
            if (shm->tables[i].chairs[j].pid != -1)     // Check if all chairs are marked as left
                all_left = false;
        }
        if (all_left) {
            // Initialize the table
            for (int j = 0; j < CHAIRS_PER_TABLE; j++)
                shm->tables[i].chairs[j].pid = 0;

            // Increment the semaphore to signal 4 chairs are available
            for (int j = 0; j < CHAIRS_PER_TABLE; j++)
                sem_post(&shm->sit);
        }
    }

    sem_post(&shm->mutex);
}

int main(int argc, char *argv[]) {
    int opt;
    int resttime = 0;  // Maximum time to rest at the table
    int shmid = 0;     // Shared memory ID

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "d:s:")) != -1) {
        switch (opt) {
            case 'd':
                resttime = atoi(optarg);
                break;
            case 's':
                shmid = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -d resttime -s shmid\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Check if the required arguments are provided
    if (resttime == 0 || shmid == 0) {
        fprintf(stderr, "Usage: %s -d resttime -s shmid\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    SharedMemory* shm = attach_shmem(shmid);    // Attach to shared memory

    // Start timer for how long the visitor stays at the bar
    struct tms tb1, tb2;
    double t1, t2, ticspersec;
    ticspersec = (double) sysconf(_SC_CLK_TCK);
    t1 = (double) times(&tb1);

    find_chair(shm);    // Search for an empty chair

    sem_post(&shm->wakeup); // Notify the receptionist that a visitor has arrived

    sem_wait(&shm->order);  // Wait for the receptionist to process the order

    // Random rest time in the range [0.7 * resttime, resttime]
    srand(time(NULL));
    int min_rest_time = (int)(0.70 * resttime);
    int rest_time = min_rest_time + rand() % (resttime - min_rest_time + 1);

    sleep(rest_time);   // Rest at the table

    leave_chair(shm);   // Leave the table

    // Stop timer for how long the visitor stays at the bar
    t2 = (double) times(&tb2);
    double visit_duration = (t2 - t1) / ticspersec;

    // Update the total visits duration
    sem_wait(&shm->mutex);
    shm->visitDuration += visit_duration;
    sem_post(&shm->mutex);

    detach_shmem(shm);  // Detach from shared memory

    return 0;
}