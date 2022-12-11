#ifndef ARG_HANDLER_H
#define ARG_HANDLER_H

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

void parse_args(int, char**);
void print_usage_and_exit(char* exe_name);
size_t copy_string(char** dst, const char* src);

#endif
