#ifndef RINGBUF_H_
#define RINGBUF_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdatomic.h>

struct ringbuf {
    _Atomic(uint64_t) head;
    _Atomic(uint64_t) tail;
    uint64_t capacity;
    char** buf;
};

void init_ringbuf(struct ringbuf* ringbuf);
// void init_ringbuf(struct ringbuf** ringbuf);
void destroy_ringbuf(struct ringbuf* ringbuf);
void push_ringbuf(struct ringbuf* ringbuf, char* item);
char* pop_ringbuf(struct ringbuf* ringbuf);

#endif
