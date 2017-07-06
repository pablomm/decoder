
#ifndef INTERFACE_H
#define INTERFACE_H

#define BAR_LEN 30

#ifdef __cplusplus

#include <stdlib.h>
#include <cstdio>

extern "C" {
#endif

ssize_t getpass_custom (char **lineptr, size_t *n, FILE *stream);

void restore_termios(FILE *stream);

void progress(int count, int total);

void progress_m(int count, int total,const char * message);

#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_H */

