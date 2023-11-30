#ifndef OUTPUT_H
#define OUTPUT_H

bool writebytes(unsigned long long x, int nbytes);
int outputText(int (*initialize)(char *path), unsigned long long (*rand64)(void), void (*finalize)(void), int nbytes, char *path);
int output_bytes(int (*initialize)(char *), unsigned long long (*rand64)(void), void (*finalize)(void), int nbytes, char *path, int N);

#endif