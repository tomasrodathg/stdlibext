#include <stdio.h>

#ifndef __PRINTLN__
#define println(FORMAT, ...) printf(FORMAT "\n", __VA_ARGS__)
#endif
