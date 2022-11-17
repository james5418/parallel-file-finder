#include "arg_handler.h"

extern char* pattern;
extern char* starting_dir;
extern int thread_num;

void parse_args(int argc, char* argv[]) {
    char opt;

    starting_dir = NULL;

    while ((opt = getopts(argc, argv, "p:d:t:")) != -1) {
        switch (opt) {
        case 'p':
            copy_string(pattern, optarg);
            break;
        case 'd':
            copy_string(starting_dir, optarg);
            break;
        case 't':
            thread_num = (int) strtol(optarg, NULL, 10);
            break;
        default:
            print_usage();
        }
    }

    if (!starting_dir) {
        starting_dir = (char*) malloc(sizeof(char) * 2);
        starting_dir[0] = '.';
        starting_dir[1] = '\0';
    }
}

void copy_string(char* dst, char* src) {
    size_t n = strlen(src) + 1;
    dst = (char*) malloc(sizeof(char) * n);
    strncpy(dst, src, n);
}

void print_usage(void) {
    fprintf(stderr, "Usage: %s -p <pattern> [-d <starting directory>] [-t <thread number>]\n", argv[0]);
    exit(1);
}