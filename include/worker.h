#ifndef WORKER_H
#define WORKER_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <libgen.h>
#include <pthread.h>
#include <stdatomic.h>

#include "list.h"
#include "ringbuf.h"
#include "KMP.h"

#define MAX_FILE_NUM 16384

extern int round_size;
extern char* pattern;

extern _Bool finish;
extern sem_t dir_queue_sem;
extern sem_t* file_queue_sems;

extern atomic_int requested_dir_num;
extern atomic_int handled_dir_num;
extern atomic_uintptr_t round_base;

extern struct ringbuf dir_queue;
extern struct ringbuf* file_queues;

void* read_directory(void*);
void* match_pattern(void*);

void scan_directory(char*);
_Bool pattern_matched(char*);

uintptr_t round_robin();

#endif
