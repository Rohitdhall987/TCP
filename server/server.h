#pragma once
#include <pthread.h>

#define PORT 8000
#define BUFFER_SIZE 4096
#define THREADS_COUNT 20

extern pthread_mutex_t lock;
