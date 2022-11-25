#ifndef WORKER_H
#define WORKER_H

#include <stdlib.h>
#include <semaphore.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <libgen.h>

#include "ringbuf.h"

#define MAX_FILE_NUM 16384

extern char* pattern;
extern char* starting_dir;

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