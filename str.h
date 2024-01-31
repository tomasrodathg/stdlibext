#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#ifndef __STR_ERRS__
#define STR_GENERIC_ERR 1
#define STR_ALLOC_ERR 2
#define STR_GROW_ERR 3
#define STR_CPY_ERR 4
#endif
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Defining a `String` as containing:												 *
 * - A pointer to the first `char` of the array of `char`'s  *
 * - A length, which is the current size of the `String`		 *
 * - A capacity, which is the maximum allocatable capacity,  *
 *   which can be expanded via calls to `grow`.							 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef enum {
	Unsafe = 0,
	Safe = 1,
} str_type;

typedef struct {
	char *s;
	str_type type;
	size_t cap;
	size_t len;
	size_t spos;
	size_t gfactor;
	void (*print)(void *);
	int (*grow)(void *, size_t);
	int (*pushstr)(void *, const char *);
} string;

string *string_from(const char*, str_type);

void print_string(string *);
void stringfree(string *);
void stringclean(void *);
