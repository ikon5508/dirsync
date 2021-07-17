#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifndef logging_h
#define logging_h

int init_log (const char *path);
int loggingf (const char *format, ...);
int close_log ();
#endif
