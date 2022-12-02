#include "KMP.h"

// Reference: https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/

_Bool KMPSearch(char* text, size_t n, char* pattern, size_t m) {
    // create lps[] that will hold the longest prefix suffix values for pattern
    int* lps = alloca(sizeof(int) * m);

    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pattern, lps, m);

    int i = 0; // index for text[]
    int j = 0; // index for pattern[]
    while ((n - i) >= (m - j)) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }

        if (j == m) {
            return true;
        }

        // mismatch after j matches
        else if (i < n && pattern[j] != text[i]) {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }

    return false;
}

// Fills lps[] for given pattern pattern[0..m-1]
void computeLPSArray(char* pattern, int* lps, size_t m) {
    // length of the previous longest prefix suffix
    int len = 0;

    lps[0] = 0; // lps[0] is always 0

    // the loop calculates lps[i] for i = 1 to m-1
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (len != 0) {
                len = lps[len - 1];

                // Also, note that we do not increment
                // i here
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}
