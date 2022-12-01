#include "main.h"

char* pattern;
char* starting_dir;
int thread_num;

_Bool finish = false;
sem_t dir_queue_sem;
sem_t file_queue_sem;

struct ringbuf dir_queue;
struct ringbuf file_queue;

struct List matched_files;

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    init();

    int dir_thread_num = thread_num / 2;
    int file_thread_num = thread_num - dir_thread_num;
    pthread_t dir_tids[dir_thread_num - 1];  // reserve 1 for the main thread
    pthread_t file_tids[file_thread_num];

    create_threads(dir_tids, read_directory);
    create_threads(file_tids, match_pattern);

    // TODO: main thread: special dir worker
    // TODO: determine when to finish

    join_threads(dir_tids, NULL);
    join_threads(file_tids, merge_lists);

    struct ListNode* curr = matched_files->head;
    for (; curr; curr = curr->next)
        puts(curr->file_path);

    return 0;
}

void init(void) {
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

    destroy_list(&matched_files);

    // TODO: free all file path strings
    // with an array of lists of pointers
}

int create_threads(pthread_t tids[], int T, void* (*work)(void*)) {
    for (int i = 0; i < T; i++)
        pthread_create(tid + i, NULL, work, NULL);
}

int join_threads(pthread_t tids[], int T, void* (*post_hook)(void*)) {
    void* res;
    for (int i = 0; i < T; i++) {
        pthread_join(tid[i], &res);
        if (post_hook)
            post_hook(res);
    }
}

void* merge_lists(void* list) {
    concat_lists(matched_files, list);
    // we just free the head and tail pointers of the list
    // not the nodes of that list
    free(list);
}