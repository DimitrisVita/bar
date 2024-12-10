
#ifndef COMMON_H
#define COMMON_H

#include <semaphore.h>
#include <sys/types.h>

#define NUM_TABLES 3
#define CHAIRS_PER_TABLE 4

typedef struct {
    int visitor_count;
    int water_count;
    int wine_count;
    int cheese_count;
    int salad_count;
    double total_wait_time;
    double total_stay_time;
    sem_t table_sem[NUM_TABLES];
    sem_t order_sem;
    sem_t mutex;
} shared_data_t;

void log_event(const char *event);

#endif // COMMON_H