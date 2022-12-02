#ifndef ARG_HANDLER_H
#define ARG_HANDLER_H

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void parse_args(int, char**);
void print_usage(char* exe_name);
void copy_string(char* dst, char* src);

#endif
