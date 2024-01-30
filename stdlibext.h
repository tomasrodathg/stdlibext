#include <stdio.h>
#include "./stack.h"
#include "./print.h"
#include "./safe.h"

#ifndef CLEANUP_DEF
#define __CLEAN_UP__() \
	void clean_up(void) { \
		freestack(stk); \
		printf("Cleaned up stack!\n"); \
	}
#endif
#ifndef SAFEMAIN_DEF
#define __SAFE_MAIN__(F) \
	__CLEAN_UP__() \
	int main(void) {\
		int res = 0; \
		stk = createstack(); \
		if((res = (F)) != 0) \
			fprintf(stderr, "Failure running main\n"); \
		clean_up(); \
		return res; \
	}
#endif
