#include <stdlib.h>
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
} StrType;

typedef struct {
	char *s;
	StrType type;
	size_t cap;
	size_t len;
	size_t spos;
	size_t gfactor;
	void (*print)(void *);
	int (*grow)(void *, size_t);
	int (*pushstr)(void *, const char *);
} Str;

void print_str(Str *);
Str *str_from(const char*, StrType);
void strfree(Str *);
void strclean(void *);
