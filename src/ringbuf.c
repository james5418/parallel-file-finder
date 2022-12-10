#include "ringbuf.h"

#include <stdio.h>

void init_ringbuf(struct ringbuf* ringbuf) {
    ringbuf->head = 0;
    ringbuf->tail = 0;
    ringbuf->capacity = SIZE;  // Default SIZE in ringbuf.h
    ringbuf->mask = SIZE - 1;
    pthread_mutex_init(&ringbuf->mutex, NULL);
    ringbuf->buf = calloc(SIZE, sizeof(char*));
}

void destroy_ringbuf(struct ringbuf* ringbuf) {
    if (!ringbuf || !ringbuf->buf) return;
    free(ringbuf->buf);
    ringbuf->buf = NULL;
    pthread_mutex_destroy(&ringbuf->mutex);
}

void push_ringbuf(struct ringbuf* ringbuf, char* item) {
    uint64_t new_tail, tail;
    while (true) {
        pthread_mutex_lock(&ringbuf->mutex);
        tail = ringbuf->tail;
        new_tail = (tail + 1) & ringbuf->mask;
        if (new_tail != ringbuf->head) {
            ringbuf->tail = new_tail;
            pthread_mutex_unlock(&ringbuf->mutex);
            break;
        }
        pthread_mutex_unlock(&ringbuf->mutex);
    }
    ringbuf->buf[tail] = item;
}

char* pop_ringbuf(struct ringbuf* ringbuf) {
    uint64_t head;
    // Cache current head val and guard increment by mutex.
    pthread_mutex_lock(&ringbuf->mutex);
    head = ringbuf->head;
    if (head == ringbuf->tail) {
        pthread_mutex_unlock(&ringbuf->mutex);
        return NULL;
    }
    ringbuf->head = (head + 1) & ringbuf->mask;
    pthread_mutex_unlock(&ringbuf->mutex);
    char* result = ringbuf->buf[head];
    ringbuf->buf[head] = NULL;
    return result;
}

_Bool is_empty_ringbuf(struct ringbuf* ringbuf) {
    bool result;
    pthread_mutex_lock(&ringbuf->mutex);
    result = ringbuf->head == ringbuf->tail;
    pthread_mutex_unlock(&ringbuf->mutex);
    return result;
}
