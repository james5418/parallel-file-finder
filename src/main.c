#include "main.h"

#include <pthread.h>

char* pattern;
char* starting_dir;
int thread_num;
int round_size;
atomic_uintptr_t round_base = ATOMIC_VAR_INIT(0);

bool finish = false;
sem_t dir_queue_sem;
sem_t* file_queue_sems;

atomic_int requested_dir_num = ATOMIC_VAR_INIT(1);
atomic_int handled_dir_num = ATOMIC_VAR_INIT(0);

struct ringbuf dir_queue;
struct ringbuf* file_queues;

struct List matched_files;

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    init();

    int file_thread_num = thread_num / 2;
    int dir_thread_num = thread_num - file_thread_num - 1;  // reserve 1 for the main thread
    pthread_t dir_tids[dir_thread_num];
    pthread_t file_tids[file_thread_num];
    init_round_robin(file_thread_num);

    create_threads(dir_tids, dir_thread_num, read_directory);
    create_threads(file_tids, file_thread_num, match_pattern);

    scan_directory(starting_dir);

    while (true) {
        int ret = sem_trywait(&dir_queue_sem);

        if (ret == 0) {
            char* dir_path = pop_ringbuf(&dir_queue);
            scan_directory(dir_path);
            free(dir_path);  // no longer need it
        } else if (ret == -1 && errno == EAGAIN) {
            int requested = atomic_load_explicit(&requested_dir_num, memory_order_relaxed);
            int handled = atomic_load_explicit(&handled_dir_num, memory_order_relaxed);
            if (requested == handled) {
                bool result = true;
                for (int i = 0; i < round_size; i++) {
                    result = result && is_empty_ringbuf(&file_queues[i]);
                }
                if (result) {
                    finish = true;

                    // wake all threads
                    for (int i = 0; i < dir_thread_num; i++) sem_post(&dir_queue_sem);
                    for (int i = 0; i < file_thread_num; i++) sem_post(&file_queue_sems[i]);

                    break;
                }
            }
        }
    }

    join_threads(dir_tids, dir_thread_num, NULL);
    join_threads(file_tids, file_thread_num, merge_lists);

    struct ListNode* curr = matched_files.head;
    for (; curr; curr = curr->next) {
        fputs(curr->file_path, stdout);
        fputc('\n', stdout);
    }

    return 0;
}

void init(void) {
    sem_init(&dir_queue_sem, 0, 0);

    init_ringbuf(&dir_queue);

    init_list(&matched_files);

    atexit(clean);
}

void init_round_robin(int size) {
    round_size = size;
    file_queues = calloc(size, sizeof(struct ringbuf));
    file_queue_sems = calloc(size, sizeof(sem_t));
    for (int i = 0; i < size; i++) {
        init_ringbuf(&file_queues[i]);
        sem_init(&file_queue_sems[i], 0, 0);
    }
}

void clean(void) {
    free(pattern);
    free(starting_dir);

    sem_destroy(&dir_queue_sem);

    destroy_ringbuf(&dir_queue);

    for (int i = 0; i < round_size; i++) {
        sem_destroy(&file_queue_sems[i]);
        destroy_ringbuf(&file_queues[i]);
    }
    free(file_queues);
    free(file_queue_sems);
    // it also frees the memory space allocated for the file paths
    destroy_list(&matched_files);
}

void create_threads(pthread_t tids[], int T, void* (*work)(void*)) {
    for (int i = 0; i < T; i++) {
        pthread_create(tids + i, NULL, work, NULL);
    }
}

void join_threads(pthread_t tids[], int T, void* (*post_hook)(void*)) {
    void* res;
    for (int i = 0; i < T; i++) {
        pthread_join(tids[i], &res);
        if (post_hook) post_hook(res);
    }
}

void* merge_lists(void* list) {
    concat_lists(&matched_files, (struct List*)list);
    // we just free the head and tail pointers of the list
    // not the nodes of that list
    free(list);
    return NULL;
}
