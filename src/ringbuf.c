#include "ringbuf.h"

#include <stdio.h>

void init_ringbuf(struct ringbuf* ringbuf) {
    ringbuf->head = ATOMIC_VAR_INIT(0);
    ringbuf->tail = ATOMIC_VAR_INIT(0);
    ringbuf->capacity = 4096;  // Default 4096
    ringbuf->buf = calloc(4096, sizeof(char*));
}

void destroy_ringbuf(struct ringbuf* ringbuf) {
    if (!ringbuf || !ringbuf->buf) return;
    free(ringbuf->buf);
    ringbuf->buf = NULL;
}

void push_ringbuf(struct ringbuf* ringbuf, char* item) {
    uint64_t head = atomic_load_explicit(&ringbuf->head, memory_order_relaxed);
    uint64_t tail = atomic_load_explicit(&ringbuf->tail, memory_order_acquire);
    uint64_t capacity = ringbuf->capacity;
    uint64_t new_tail = (tail + 1) & (capacity - 1);  // Mask new tail.

    if (new_tail == head) {
        return;
    }
    // atomic_store_explicit(&ringbuf->tail, new_tail, memory_order_release);
    // ringbuf->buf[tail] = item;
    _Bool cex_result = atomic_compare_exchange_strong_explicit(&ringbuf->tail, &tail, new_tail, memory_order_release,
                                                               memory_order_acquire);
    while (!cex_result) {
        head = atomic_load_explicit(&ringbuf->head, memory_order_relaxed);
        new_tail = (tail + 1) & (capacity - 1);
        if (new_tail == head) {
            return;
        }

        cex_result = atomic_compare_exchange_strong_explicit(&ringbuf->tail, &tail, new_tail, memory_order_release,
                                                             memory_order_acquire);
    }
    ringbuf->buf[tail] = item;
}

char* pop_ringbuf(struct ringbuf* ringbuf) {
    uint64_t tail = atomic_load_explicit(&ringbuf->tail, memory_order_relaxed);
    uint64_t head = atomic_load_explicit(&ringbuf->head, memory_order_acquire);
    uint64_t capacity = ringbuf->capacity;

    if (head == tail) {  // empty queue
        return NULL;
    }

    uint64_t new_head = (head + 1) & (capacity - 1);
    // atomic_store_explicit(&ringbuf->head, new_head, memory_order_release);
    // char* result = ringbuf->buf[head];
    // return result;
    _Bool cex_result = atomic_compare_exchange_strong_explicit(&ringbuf->head, &head, new_head, memory_order_release,
                                                               memory_order_acquire);
    while (!cex_result) {
        tail = atomic_load_explicit(&ringbuf->tail, memory_order_relaxed);
        if (head == tail) {  // empty queue
            return NULL;
        }

        new_head = (head + 1) & (capacity - 1);
        cex_result = atomic_compare_exchange_strong_explicit(&ringbuf->head, &head, new_head, memory_order_release,
                                                             memory_order_acquire);
    }
    char* result = ringbuf->buf[head];
    ringbuf->buf[head] = NULL;
    return result;
}

_Bool is_empty_ringbuf(struct ringbuf* ringbuf) {
    uint64_t head = atomic_load_explicit(&ringbuf->head, memory_order_acquire);
    uint64_t tail = atomic_load_explicit(&ringbuf->tail, memory_order_acquire);
    return head == tail;
}
