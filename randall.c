/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <cpuid.h>
#include <errno.h>
#include <immintrin.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "options.h"
#include "output.h"
#include "rand64-hw.h"
#include "rand64-sw.h"


/* Hardware implementation.  */

/* Description of the current CPU.  */
struct cpuid { unsigned eax, ebx, ecx, edx; };

/* Return information about the CPU.  See <http://wiki.osdev.org/CPUID>.  */
static struct cpuid
cpuid (unsigned int leaf, unsigned int subleaf)
{
  struct cpuid result;
  asm ("cpuid"
       : "=a" (result.eax), "=b" (result.ebx),
	 "=c" (result.ecx), "=d" (result.edx)
       : "a" (leaf), "c" (subleaf));
  return result;
}

/* Return true if the CPU supports the RDRAND instruction.  */
static _Bool
rdrand_supported (void)
{
  struct cpuid extended = cpuid (1, 0);
  return (extended.ecx & bit_RDRND) != 0;
}

/* Initialize the hardware rand64 implementation.  */



/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  int nbytes = check(argc, argv);
  if (nbytes <= 0){
      fprintf(stderr, "%s: usage: %s [options] NBYTES\n", argv[0], argv[0]);
      return 1;
  }
  int opt;
  char *input = "none";
  int output_nbytes = 0; // Variable to store the N value

  while ((opt = getopt(argc, argv, "i:o:")) != -1) {
      switch (opt) {
          case 'i':
              input = optarg;
              break;
          case 'o':
              if(strcmp(optarg, "stdio") != 0){
                output_nbytes = atoi(optarg);
                if (output_nbytes <= 0) {
                  fprintf(stderr, "Invalid option for -o. Must be an integer or stdio\n");
                  return 1;
                }
              }
              break;
          default:
              fprintf(stderr, "Usage: %s -i input -o N\n", argv[0]);
              return 1;
      }
  }

  int (*initialize) (char *path);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  char *path = "";

  // Check the input option
  if(strcmp(input, "none") == 0 || strcmp(input, "rdrand") == 0){
    if (rdrand_supported ())
    {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    } else{
      fprintf(stderr, "rdrand not available\n");
      return 1;
    }
  } else if (strcmp(input, "lrand48_r") == 0) {
      initialize = lrand48_init;
      rand64 = lrand48_rand64;
      finalize = lrand48_fini;
  } else if (input[0] == '/') {
      initialize = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini;
      path = input;
  } else {
      fprintf(stderr, "Invalid input option\n");
      return 1;
  }


  if (output_nbytes == 0) {
      return outputText(initialize, rand64, finalize, nbytes, path);
  } else {
    //printf("nbytes: %d, output_nbytes: %d", nbytes, output_nbytes);
    return output_bytes(initialize, rand64, finalize, nbytes, path, output_nbytes);
      // Add your implementation for N bytes output
      // output(hardware_rand64_init, hardware_rand64, hardware_rand64_fini, nbytes);
  }
}
