#ifndef RINGBUF_H_
#define RINGBUF_H_

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define SIZE 65536

struct ringbuf {
    uint64_t head __attribute__((aligned(64)));
    uint64_t tail __attribute__((aligned(64)));
    uint64_t capacity;
    uint64_t mask;
    pthread_mutex_t mutex;
    char** buf;
};

void init_ringbuf(struct ringbuf* ringbuf);
void destroy_ringbuf(struct ringbuf* ringbuf);
void push_ringbuf(struct ringbuf* ringbuf, char* item);
char* pop_ringbuf(struct ringbuf* ringbuf);
_Bool is_empty_ringbuf(struct ringbuf* ringbuf);

#endif
