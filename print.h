#include <stdio.h>
#include <ctype.h>

#ifndef __PRINTLN__
#define println(FORMAT, ...) printf(FORMAT "\n", __VA_ARGS__)
#endif

#ifndef __EMPTY_OR_VAL__
#define empty_or_val(V) ((V) == NULL || (V)[0] == '\0') ? "{\\0}" : (V)
#endif
