#include <stdio.h>
#include <stdlib.h>
#include "safe.h"
#include "log.h"

static int __allocerr = 0;
static int __allocerr_reason = 0;
static void *localalloc[__LOCAL_ALLOC_SIZE] = {0};
static int safe = 0;
static size_t allocidx = 0;
static galloc *global_alloc = {0};

void __init_global_alloc() {
	if(!(global_alloc = malloc(sizeof(galloc)))) exit(1);
}

void __registerret(void *ret, void *retbuf[1])
{
	retbuf[0] = ret;
}

void *__getregisteredret(void *retbuf[1])
{
	return retbuf[0];
}

void __setunsafe(void)
{
	safe = 0;
}

void __setsafe(void)
{
	safe = 1;
}

void __clearlocalalloc(void)
{
	for(int i = 0; i < __LOCAL_ALLOC_SIZE; i++) localalloc[i] = NULL;
}

int __issafe(void)
{
	return safe;
}

void *__pushtolocalalloc(void *ptr)
{
	localalloc[allocidx++] = ptr;
	logln(INFO, "|Global Allocator| Allocated memory safely and added it to the stack.");
	return ptr;
}

void *__popfromlocalalloc(void)
{
	void *ptr;
	if(allocidx > 0 && ((ptr = localalloc[--allocidx]))) return ptr;
	return NULL;
}

int *__getallocerr(void)
{
	return &__allocerr;
}

int *__getallocerr_reason(void)
{
	return &__allocerr_reason;
}

void __freelocalalloc(void)
{
	void *tmpptr;
	size_t count = 0;
	while((tmpptr = __popfromlocalalloc()) != NULL) {
		++count;
		free(tmpptr);
		tmpptr = NULL;
	}

	(count == 0) ? 
	logln(INFO, "|Global Allocator| Nothing to clear.") :
	logln(INFO, "|Global Allocator| Freed memory safely. Cleared %lu objects", count);
}	

void *__smalloc(size_t size)
{
	void *ptr;
	if(!(ptr = malloc(size))) goto cleanup;
	if(allocidx < __LOCAL_ALLOC_SIZE) return __pushtolocalalloc(ptr);
	__allocerr_reason = LOCAL_ALLOC_SIZE_EXCEEDED;

cleanup:
	__allocerr = EMALLOC;
	void *tmpptr;
	while((tmpptr = __popfromlocalalloc()) != NULL) free(tmpptr);
	return NULL;
}
