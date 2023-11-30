#include "options.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int check(int argc, char **argv) {
    bool valid = false;
    long long nbytes = 0;

    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "-o") == 0) && i + 1 < argc) {
            // Check for -i or -o followed by a string
            ++i;
        } else {
            // Check for NBYTES
            char *endptr;
            errno = 0;
            nbytes = strtoll(argv[i], &endptr, 10);

            if (errno == 0 && *endptr == '\0') {
                valid = true;
                break;
            }
        }
    }

    if (!valid) {
        fprintf(stderr, "%s: usage: %s [options] NBYTES\n", argv[0], argv[0]);
        return -1;
    }

    return nbytes;
}