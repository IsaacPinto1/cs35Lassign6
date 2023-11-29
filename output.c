#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include "output.h"

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

int output(void (*initialize)(void), unsigned long long (*rand64)(void), void (*finalize)(void), int nbytes){
    initialize ();
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