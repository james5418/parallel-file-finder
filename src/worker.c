#include "worker.h"

void* read_directory(void* data) {
    char *dir_path;

    while(!finish){
        sem_wait(&dir_queue_sem);
        if (finish) break;
        
        dir_path = pop_ringbuf(&dir_queue);
        scan_directory(dir_path);
    }

    pthread_exit(0);
}

void scan_directory(char *dir_path){
    DIR *dir;
    struct dirent* dent;

    dir = opendir(dir_path);
    while((dent = readdir(dir)) != NULL){
        char *name = dent->d_name;
        if((strcmp(name, ".")==0) || (strcmp(name, "..")==0)) continue;

        char *entry_path = (char*)malloc(PATH_MAX);
        sprintf(entry_path, "%s/%s", dir_path, name);

        struct stat st;
        stat(entry_path, &st);

        if(S_ISDIR(st.st_mode)){
            push_ringbuf(&dir_queue, entry_path);
        }
        else{
            push_ringbuf(&file_queue, entry_path);
        }
    }
    closedir(dir);
}


void* match_pattern(void* data) {
    struct List* matched_files_local = (struct List*) malloc(sizeof(struct List));
    init_list(matched_files_local);

    while (!finish) {
        sem_wait(&file_queue_sem);
        if (finish) break;

        char* file_path = pop_ringbuf(&file_queue);
        if (pattern_matched(file_path))
            append_list(matched_files_local, file_path);
    }

    return matched_files_local;
}


_Bool pattern_matched(char* file_path) {
    // extract filename from the path
    char* filename = basename(file_path);

    size_t p_len = strlen(pattern);
    size_t f_len = strlen(filename);

    // *text*
    if (pattern[0] == '*' && pattern[p_len - 1] == '*') {
        // TODO: KMP or 2-way algorithm

    // *text
    } else if (pattern[0] == '*') {
        // -1 to exclude the '*'
        if (p_len - 1 > f_len)
            return false;

        int i = f_len - 1, j = p_len - 1;
        while (j >= 0 && filename[i] == pattern[j]) {
            i--;
            j--;
        }

        return j == -1;

    // text*
    } else if (pattern[p_len - 1] == '*') {
        // -1 to exclude the '*'
        return (p_len - 1) <= f_len ? strncmp(filename, pattern, p_len - 1) : false;

    // text
    } else {
        return p_len == f_len ? (strcmp(filename, pattern) == 0) : false;
    }
}