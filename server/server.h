#pragma once
#include <pthread.h>

#define PORT 8000
#define BUFFER_SIZE 4096
#define THREADS_COUNT 60

extern pthread_mutex_t lock;

extern pthread_cond_t condition;
