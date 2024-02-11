#include <stdlib.h>
#ifndef __STR_H__
#define __STR_H__
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
 * - A growth factor, which for stays a (*2), it doubles the *
 *   difference between current size and new size to calc	   *
 *   the new capacity																				 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct {
	char *ptr;
	size_t cap;
	size_t len;
	size_t gfactor;
	void (*print)(void *);
} Str;

Str *str_from(const char*);
int push_str_safe(Str *, const char *);
void print_str(Str *);
void str_free(Str *);
void clear(Str *);
#endif
