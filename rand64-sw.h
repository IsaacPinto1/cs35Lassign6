#ifndef RAND64_SW_H
#define RAND64_SW_H

#include <stdio.h>

int software_rand64_init(char *path);
unsigned long long software_rand64(void);
void software_rand64_fini(void);

int lrand48_init(char *path);
unsigned long long lrand48_rand64(void);
void lrand48_fini(void);

#endif /* RAND64_SW_H */