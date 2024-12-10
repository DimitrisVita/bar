
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void log_event(const char *event) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    printf("[%02d:%02d:%02d] %s\n", t->tm_hour, t->tm_min, t->tm_sec, event);
}