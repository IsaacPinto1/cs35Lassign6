#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include "output.h"
#include <unistd.h>

bool writebytes (unsigned long long x, int nbytes)
{
  do
    {
      if (putchar (x) < 0)
	return false;
      x >>= CHAR_BIT;
      nbytes--;
    }
  while (0 < nbytes);

  return true;
}

int outputText(int (*initialize)(char *path), unsigned long long (*rand64)(void), void (*finalize)(void), int nbytes, char *path){
    if(!!initialize(path) == 1){
        return 1;
    };
    int wordsize = sizeof rand64 ();
    int output_errno = 0;

    do{
        unsigned long long x = rand64 ();
        int outbytes = nbytes < wordsize ? nbytes : wordsize;
        if (!writebytes (x, outbytes)){
            output_errno = errno;
            break;
        }
        nbytes -= outbytes;
    }
    while (0 < nbytes);

    if (fclose (stdout) != 0)
        output_errno = errno;

    if (output_errno){
        errno = output_errno;
        perror ("output");
    }

    finalize ();
    return !!output_errno;
}


int output_bytes(int (*initialize)(char *path), unsigned long long (*rand64)(void), void (*finalize)(void), int total_bytes, char *path, int N) {
    if(!!initialize(path) == 1) {
        return 1;
    }
    int output_errno = 0;

    // Allocate a buffer for N bytes
    char *buffer = (char *)malloc(N);
    if (buffer == NULL) {
        perror("Error allocating memory");
        finalize();
        return 1;
    }

    do {
        unsigned long long x = rand64();

        // Fill the buffer with N random bytes or the remaining bytes in the last iteration
        int bytes_to_write = (total_bytes < N) ? total_bytes : N;

        for (int i = 0; i < bytes_to_write; ++i) {
            buffer[i] = (char)(x & 0xFF);
            x >>= 8;
        }

        // Write the buffer to stdout
        int bytes_written = write(STDOUT_FILENO, buffer, bytes_to_write);
        if (bytes_written != bytes_to_write) {
            output_errno = errno;
            break;
        }

        total_bytes -= bytes_to_write;
    } while (total_bytes > 0);

    free(buffer);

    // Check for errors and finalize
    if (output_errno) {
        errno = output_errno;
        perror("output");
        finalize();
        return 1;
    }

    if (fclose(stdout) != 0) {
        perror("Error closing stdout");
        finalize();
        return 1;
    }

    finalize();
    return 0;
}