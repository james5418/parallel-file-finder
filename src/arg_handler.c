#include "arg_handler.h"

extern char *pattern;
extern char *starting_dir;
extern int thread_num;

void parse_args(int argc, char *argv[]) {
    char opt;

    if (argc < 2)
        print_usage_and_exit(argv[0]);

    pattern = NULL;
    starting_dir = NULL;
    thread_num = 4;

    size_t starting_dir_len = 0;

    while ((opt = getopt(argc, argv, "p:d:t:")) != -1) {
        switch (opt) {
        case 'p':
            copy_string(&pattern, optarg);
            break;
        case 'd':
            starting_dir_len = copy_string(&starting_dir, optarg);
            break;
        case 't':
            thread_num = (int) strtol(optarg, NULL, 10);
            break;
        default:
            print_usage_and_exit(argv[0]);
        }
    }

    if (!pattern)
        print_usage_and_exit(argv[0]);

    if (!starting_dir) {
        starting_dir = (char *) malloc(sizeof(char) * 2);
        starting_dir[0] = '.';
        starting_dir[1] = '\0';
    }

    // if dir not exist
    struct stat stats;
    stat(starting_dir, &stats);
    if (!S_ISDIR(stats.st_mode)) {
        fprintf(stderr, "Directory does not exist: %s\n", starting_dir);
        exit(1);
    }

    // remove trailing slash for the directory
    int end = starting_dir_len - 1;
    if (starting_dir[end] == '/') {
        starting_dir[end] = '\0';
    }
}

size_t copy_string(char **dst, const char *src) {
    size_t n = strlen(src) + 1;
    *dst = (char *) malloc(sizeof(char) * n);
    strncpy(*dst, src, n);
    return n - 1;
}

void print_usage_and_exit(char *exe_name) {
    fprintf(stderr, "Usage: %s -p <pattern> [-d <starting directory>] [-t <thread number>]\n", exe_name);
    exit(1);
}
