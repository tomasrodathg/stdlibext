#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#ifndef __LOG_H__
#define __LOG_H__
typedef enum {
	DEBUG,
	INFO,
	WARN,
	ERROR,
} loglevel;
extern void __log_with_level(loglevel l, ...);
extern const char *strloglevel(loglevel l);
#define logln(L,...) __log_with_level(L, __VA_ARGS__)
#endif
