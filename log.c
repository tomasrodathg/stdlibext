#include "log.h"
#include <stdarg.h>

const char *strloglevel(loglevel l)
{
	switch (l) {
		case DEBUG:
			return  "[DEBUG]: ";
		case INFO:
			return "[INFO]: ";
		case WARN:
			return "[WARN]: ";
		case ERROR:
			return "[ERROR]: ";
		default:
			return "";
	}
}

void __log_with_level(loglevel l,...) {
    va_list args;
    va_start(args, l);
		const char* fmt = va_arg(args, const char*);
    const char* prefix = strloglevel(l);
    fprintf((l == ERROR) ? stderr : stdout, "%s", prefix);
    vfprintf((l == ERROR) ? stderr : stdout, fmt, args);
    fprintf((l == ERROR) ? stderr : stdout, "\n");
    va_end(args);
}
