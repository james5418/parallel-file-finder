#ifndef KMP_H
#define KMP_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Reference: https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/

_Bool KMPSearch(char* text, size_t n, char* pattern, size_t m);
static void computeLPSArray(char* pat, int* lps, size_t m);

#endif