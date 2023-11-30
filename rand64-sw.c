#include "rand64-sw.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


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

int arc4random_init(char *path __attribute__((unused))) {
    // Your initialization code here
}

// Function to generate a random 64-bit number using arc4random_buf
unsigned long long arc4random_rand64(void) {
    unsigned long long result;
    arc4random_buf(&result, sizeof(result));
    return result;
}

// Function to finalize arc4random
void arc4random_fini(void) {
    // Your finalization code here
}