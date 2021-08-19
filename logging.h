#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>



#ifndef logging_h
#define logging_h

extern int debug;

int init_log (const char *path);
int loggingf (const int level, const char *format, ...);
void close_log ();

#define remove_old 1

#endif
