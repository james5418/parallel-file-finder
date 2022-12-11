#include "ringbuf.h"

#include <stdio.h>

void init_ringbuf(struct ringbuf* ringbuf) {
    ringbuf->head = 0;
    ringbuf->tail = 0;
    ringbuf->capacity = SIZE;  // Default SIZE in ringbuf.h
    ringbuf->mask = SIZE - 1;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&ringbuf->mutex, &attr);
    pthread_cond_init(&ringbuf->writable, NULL);
    ringbuf->buf = calloc(SIZE, sizeof(char*));
}

void destroy_ringbuf(struct ringbuf* ringbuf) {
    if (!ringbuf || !ringbuf->buf) return;
    free(ringbuf->buf);
    ringbuf->buf = NULL;
    pthread_mutex_destroy(&ringbuf->mutex);
    pthread_cond_destroy(&ringbuf->writable);
}

void push_ringbuf(struct ringbuf* ringbuf, char* item) {
    pthread_mutex_lock(&ringbuf->mutex);
    while (((ringbuf->tail + 1) & ringbuf->mask) == ringbuf->head) {
        pthread_cond_wait(&ringbuf->writable, &ringbuf->mutex);
    }
    ringbuf->buf[ringbuf->tail] = item;
    ringbuf->tail = (ringbuf->tail + 1) & ringbuf->mask;
    pthread_mutex_unlock(&ringbuf->mutex);
}

char* pop_ringbuf(struct ringbuf* ringbuf) {
    uint64_t head;
    char* result;
    // Cache current head val and guard increment by mutex.
    pthread_mutex_lock(&ringbuf->mutex);
    head = ringbuf->head;
    if (head == ringbuf->tail) {
        pthread_mutex_unlock(&ringbuf->mutex);
        return NULL;
    }
    ringbuf->head = (head + 1) & ringbuf->mask;
    result = ringbuf->buf[head];
    ringbuf->buf[head] = NULL;
    pthread_mutex_unlock(&ringbuf->mutex);
    return result;
}

bool is_empty_ringbuf(struct ringbuf* ringbuf) {
    bool result;
    pthread_mutex_lock(&ringbuf->mutex);
    result = ringbuf->head == ringbuf->tail;
    pthread_mutex_unlock(&ringbuf->mutex);
    return result;
}
