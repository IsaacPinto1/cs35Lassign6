#include "rand64-sw.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


FILE *urandstream;

int software_rand64_init(char *path) {
    urandstream = fopen(path, "r");
    if (!urandstream) {
        perror("Error opening random file");
        return 1;
    } else {
      unsigned long long int x;
      if (fread (&x, sizeof x, 1, urandstream) != 1){
        fprintf(stderr, "Error reading from the given file\n");
        return 1;
      }
    }
    return 0;
}

/* Return a random value, using software operations.  */
unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  fread (&x, sizeof x, 1, urandstream);
  return x;
}

/* Finalize the software rand64 implementation.  */
void
software_rand64_fini (void)
{
  fclose (urandstream);
}





int lrand48_init(char *path __attribute__((unused))) {
    // Seed the random number generator
    srand48((long)getpid());  // You may want to use a different seed value

    return 0;
}

unsigned long long lrand48_rand64(void) {
    // Generate a 32-bit random number using lrand48
    unsigned long rand32 = lrand48();

    // Extend to 64 bits
    //unsigned long long rand64 = (unsigned long long)rand32;

    return rand32;
}

void lrand48_fini(void) {
    // No specific finalization needed for lrand48
}