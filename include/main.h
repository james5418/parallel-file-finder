#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include "arg_handler.h"
#include "ringbuf.h"
#include "list.h"
#include "worker.h"

void init(void);
void clean(void);

void create_threads(pthread_t tids[], int T, void* (*work)(void*));
void join_threads(pthread_t tids[], int T, void* (*post_hook)(void*));

void* merge_lists(void* list);

#endif
