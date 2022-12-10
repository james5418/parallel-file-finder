#ifndef RINGBUF_H_
#define RINGBUF_H_

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define SIZE 65536  // it should be power of 2

struct ringbuf {
    uint64_t head __attribute__((aligned(64)));
    uint64_t tail __attribute__((aligned(64)));
    uint64_t capacity __attribute__((aligned(64)));
    uint64_t mask __attribute__((aligned(64)));
    pthread_mutex_t mutex __attribute__((aligned(64)));
    char** buf __attribute__((aligned(64)));
};

void init_ringbuf(struct ringbuf* ringbuf);
void destroy_ringbuf(struct ringbuf* ringbuf);
void push_ringbuf(struct ringbuf* ringbuf, char* item);
char* pop_ringbuf(struct ringbuf* ringbuf);
bool is_empty_ringbuf(struct ringbuf* ringbuf);

#endif
