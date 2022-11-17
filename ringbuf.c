#include "ringbuf.h"
#include <stdlib.h>

void init_ringbuf(struct ringbuf* ringbuf) {
    ringbuf = malloc(sizeof(struct ringbuf));
    ringbuf->head = 0;
    ringbuf->tail = 0;
    ringbuf->capacity = 4096; // Default 4096
    ringbuf->buf = malloc(sizeof(char*) * 4096);
}
void push_ringbuf(struct ringbuf* ringbuf, char* item) {
    uint64_t head = atomic_load_explicit(&ringbuf->head, __ATOMIC_RELAXED);
    uint64_t tail = atomic_load_explicit(&ringbuf->tail, __ATOMIC_ACQUIRE);
    uint64_t capacity = ringbuf->capacity;
    if (tail - head < capacity) {
        ringbuf->buf[tail] = item;
        uint64_t new_tail = (tail + 1) & (capacity - 1); // Mask new tail.
        atomic_store_explicit(&ringbuf->tail, new_tail, __ATOMIC_RELEASE);
    }
}
char* pop_ringbuf(struct ringbuf* ringbuf) {

}