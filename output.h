#ifndef OUTPUT_H
#define OUTPUT_H

bool writebytes(unsigned long long x, int nbytes);
int output(void (*initialize)(void), unsigned long long (*rand64)(void), void (*finalize)(void), int nbytes);

#endif