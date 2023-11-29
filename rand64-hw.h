/* rand64-hw.h */

#ifndef RAND64_HW_H
#define RAND64_HW_H

void hardware_rand64_init(void);
unsigned long long hardware_rand64(void);
void hardware_rand64_fini(void);

#endif /* RAND64_HW_H */