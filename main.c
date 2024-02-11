#include <stdio.h>
#include "str.h"
#include "vec.h"
#include "safe.h"
#include <string.h>


char *func(const char *str)
{
__BEGIN_SAFE
	Str *s = str_from(str);
	if (allocerr) fprintf(stderr, "allocation error: %d\n", allocerr_reason);
	printf("%s\n", s->ptr);
	RETURN(s);
__END_SAFE
}


int smain(void)
{
	func("This is one string");
	return 0;
}
__GLOBAL_ALLOC(smain)
