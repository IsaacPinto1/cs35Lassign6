#ifndef RAND64_SW_H
#define RAND64_SW_H

#include <stdio.h>

int software_rand64_init(char *path);
unsigned long long software_rand64(void);
void software_rand64_fini(void);

int arc4random_init(char *path);
unsigned long long arc4random_rand64(void);
void arc4random_fini(void);

#endif /* RAND64_SW_H */