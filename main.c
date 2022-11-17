#include <stdlib.h>
#include <pthread.h>

#include "arg_handler.h"

void clean(void);

char* pattern;
char* starting_dir;
int thread_num;


int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    atexit(clean);

    pthread_t pids[thread_num];




}

void clean(void) {
    free(pattern);
    free(starting_dir);
}

int create_threads(pthread_t pids[], char* pattern) {
    
}

int join_threads(pthread_t pids[]) {

}