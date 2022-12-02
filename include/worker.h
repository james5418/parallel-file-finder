#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <libgen.h>
#include <pthread.h>

#include "list.h"
#include "ringbuf.h"
#include "KMP.h"

#define MAX_FILE_NUM 16384

extern char* pattern;

extern _Bool finish;
extern sem_t dir_queue_sem;
extern sem_t file_queue_sem;

extern struct ringbuf dir_queue;
extern struct ringbuf file_queue;

void* read_directory(void*);
void* match_pattern(void*);

void scan_directory(char*);
_Bool pattern_matched(char*);

#endif
