#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <semaphore.h>

#define NUM_TABLES 3
#define CHAIRS_PER_TABLE 4
#define MAX_VISITORS 100

typedef struct {
    int table_id;
    int chairs[CHAIRS_PER_TABLE];
    int visitor_count;
} Table;

typedef struct {
    Table tables[NUM_TABLES];
    int total_visitors;
    int water_count;
    int wine_count;
    int cheese_count;
    int salad_count;
    double total_wait_time; // Add total_wait_time field
    double total_stay_time;
    sem_t table_sem[NUM_TABLES];
    sem_t waiting_sem;
    sem_t order_sem;
    sem_t mutex;
    int table_status[3][4]; // 3 tables with 4 chairs each
    sem_t table_semaphores[3]; // Semaphore for each table
    sem_t buffer_sem; // Semaphore for circular buffer
    int visitor_pids[MAX_VISITORS]; // Array to store visitor PIDs
} SharedMemory;

int create_shared_memory();
SharedMemory* attach_shared_memory(int shmid);
void detach_shared_memory(SharedMemory* shm);
void destroy_shared_memory(int shmid);
void initialize_semaphores(SharedMemory* shm);
void destroy_semaphores(SharedMemory* shm);

#endif // SHARED_MEMORY_H