#include "options.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int check(int argc, char **argv) {
    int numeric_arg_count = 0;
    int nbytes = 0; // Variable to store the N value

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 < argc) {
                ++i;  // Skip the option argument
            } else {
                // Missing option argument
                return -1;
            }
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                ++i;  // Skip the option argument
            } else {
                // Missing option argument
                return -1;
            }
        } else {
            // Check for NBYTES
            char *endptr;
            errno = 0;
            long long int temp = strtoll(argv[i], &endptr, 10);

            if (errno == 0 && *endptr == '\0') {
                numeric_arg_count++;
                nbytes = temp;
            } else {
                // Invalid argument
                return -1;
            }
        }
    }

    if (numeric_arg_count != 1) {
        return -1;
    }

    return nbytes;
}