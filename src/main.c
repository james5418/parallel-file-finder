#include "main.h"

#include <pthread.h>

char* pattern;
char* starting_dir;
int thread_num;

_Bool finish = false;
sem_t dir_queue_sem;
sem_t file_queue_sem;

int requested_dir_num = 1;
int handled_dir_num = 0;

struct ringbuf dir_queue;
struct ringbuf file_queue;

struct List matched_files;

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    init();

    int file_thread_num = thread_num / 2;
    int dir_thread_num = thread_num - file_thread_num - 1;  // reserve 1 for the main thread
    pthread_t dir_tids[dir_thread_num];
    pthread_t file_tids[file_thread_num];

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
            if (requested_dir_num == handled_dir_num && is_empty_ringbuf(&file_queue)) {
                finish = true;

                // wake all threads
                for (int i = 0; i < dir_thread_num; i++) sem_post(&dir_queue_sem);
                for (int i = 0; i < file_thread_num; i++) sem_post(&file_queue_sem);

                break;
            }
        }
    }

    join_threads(dir_tids, dir_thread_num, NULL);
    join_threads(file_tids, file_thread_num, merge_lists);

    struct ListNode* curr = matched_files.head;
    for (; curr; curr = curr->next) puts(curr->file_path);

    return 0;
}

void init(void) {
    thread_num = 4;

    sem_init(&dir_queue_sem, 0, 0);
    sem_init(&file_queue_sem, 0, 0);

    init_ringbuf(&dir_queue);
    init_ringbuf(&file_queue);

    init_list(&matched_files);

    atexit(clean);
}

void clean(void) {
    free(pattern);
    free(starting_dir);

    sem_destroy(&dir_queue_sem);
    sem_destroy(&file_queue_sem);

    destroy_ringbuf(&dir_queue);
    destroy_ringbuf(&file_queue);

    // it also frees the memory space allocated for the file paths
    destroy_list(&matched_files);
}

void create_threads(pthread_t tids[], int T, void* (*work)(void*)) {
    for (int i = 0; i < T; i++) pthread_create(tids + i, NULL, work, NULL);
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
