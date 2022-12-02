#include "ringbuf.h"

void init_ringbuf(struct ringbuf* ringbuf) {
// void init_ringbuf(struct ringbuf** ringbuf) {
    // struct ringbuf* rb = calloc(1, sizeof(struct ringbuf));
    // rb->head = ATOMIC_VAR_INIT(0);
    // rb->tail = ATOMIC_VAR_INIT(0);
    // rb->capacity = 4096; // Default 4096
    // rb->buf = calloc(4096, sizeof(char*));
    // *ringbuf = rb;
    ringbuf->head = ATOMIC_VAR_INIT(0);
    ringbuf->tail = ATOMIC_VAR_INIT(0);
    ringbuf->capacity = 4096; // Default 4096
    ringbuf->buf = calloc(4096, sizeof(char*));
}

void destroy_ringbuf(struct ringbuf* ringbuf) {
    if (ringbuf) {
        if (ringbuf->buf) {
            free(ringbuf->buf);
        }
        // free(ringbuf);
    }
}

void push_ringbuf(struct ringbuf* ringbuf, char* item) {
    uint64_t head = atomic_load_explicit(&ringbuf->head, memory_order_relaxed);
    uint64_t tail = atomic_load_explicit(&ringbuf->tail, memory_order_acquire);
    uint64_t capacity = ringbuf->capacity;
    uint64_t sz;

    if (head <= tail) {
        // |-----h+++++t-------|
        // +: filled slot
        // -: empty slot
        sz = tail - head;
    } else {
        // |+++++t-----h+++++++|
        // +: filled slot
        // -: empty slot
        sz = capacity - (head - tail);
    }

    if (sz < capacity) {
        uint64_t new_tail = (tail + 1) & (capacity - 1); // Mask new tail.
        atomic_store_explicit(&ringbuf->tail, new_tail, memory_order_release);
        ringbuf->buf[tail] = item;
    }
}

char* pop_ringbuf(struct ringbuf* ringbuf) {
    uint64_t tail = atomic_load_explicit(&ringbuf->tail, memory_order_relaxed);
    uint64_t head = atomic_load_explicit(&ringbuf->head, memory_order_acquire);
    uint64_t capacity = ringbuf->capacity;

    if (head == tail) { // empty queue
        return NULL;
    }

    uint64_t new_head = (head + 1) & (capacity - 1);
    atomic_store_explicit(&ringbuf->head, new_head, memory_order_release);
    char* result = ringbuf->buf[head];
    return result;
}
