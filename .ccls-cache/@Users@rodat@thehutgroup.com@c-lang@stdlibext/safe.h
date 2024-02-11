 /* A 'Safe Wrapper' around some of C's main features.
 *
 * The goal is to minimize unsafe behaviour while also ensure that C's
 * rawness, low overhead runtime and minimalistic approach - as well as
 * the power of manual management of resources - are preserved
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#ifndef __SAFE_H
#define __SAFE_H
#ifndef __SAFE_ERR
#define EMALLOC 1
#define EBUFSIZE 2
#endif
#ifndef __SAFE_ERR_REASONS
#define MALLOC_FAILURE 1
#define LOCAL_ALLOC_SIZE_EXCEEDED 2
#endif
#ifndef __GLOBAL_ALLOC_H
#define __GLOBAL_ALLOC_H
#define __LOCAL_ALLOC_SIZE 20
#define __LOCAL_ALLOC_POOL_SIZE 20
typedef struct {
	void *lallocbuf[__LOCAL_ALLOC_SIZE];
	size_t nmemb;
} lalloc;
typedef struct {
	lalloc *lallocpool[__LOCAL_ALLOC_POOL_SIZE];
	size_t nlalloc;
} galloc;
extern void __init_global_alloc(void);
extern void __add_local_alloc(lalloc *);
extern void __setunsafe(void);
extern void __setsafe(void);
extern void __clearlocalalloc(void);
extern void __freelocalalloc(void);
extern void *__popfromlocalalloc(void);
extern void *__pushtolocalalloc(void *);
extern int __issafe(void);
extern int *__getallocerr(void);
extern int *__getallocerr_reason(void);
extern int *__setlocalallocsize(size_t size);
extern void *__getregisteredret(void *retbuf[1]);
extern void __registerret(void *ret, void * retbuf[1]);
#define RETURN(VAL) __registerret(VAL, retbuf);
#define __REGISTERED __getregisteredret(retbuf)
#define __SET_UNSAFE (__setunsafe())
#define __SET_SAFE (__setsafe())
#define __IS_SAFE (*__issafe())
#define __FREE_LOCAL_ALLOC (__freelocalalloc())
#define __CLEAR_LOCAL_ALLOC (__clearlocalalloc())
#define __INIT_GLOBAL_ALLOC (__init_global_alloc())
#define __BEGIN_SAFE \
	__SET_SAFE; \
	void *retbuf[1] = {0};
#define __END_SAFE \
	__FREE_LOCAL_ALLOC; \
	__SET_UNSAFE; \
	return __REGISTERED; \

#define __UNSAFE(RET,FNAME,PARAMS,ARGS,FBODY)\
RET FNAME PARAMS { FBODY }\
RET unsafe_##FNAME PARAMS { __SET_UNSAFE; RET tmp = FNAME ARGS; __SET_SAFE; return tmp; }\

#define __GLOBAL_ALLOC(F) \
	int main(void) \
	{ \
		__INIT_GLOBAL_ALLOC; \
		int __main_res; \
		__SET_SAFE; 			\
		__main_res = (F)(); \
		__FREE_LOCAL_ALLOC; \
		return __main_res; \
	}
#endif
#ifndef __SMALLOC__
extern void *__smalloc(size_t);
#define allocerr (*__getallocerr())
#define allocerr_reason (*__getallocerr_reason())
#define smalloc(SIZEOF) (__issafe()) ? (__smalloc((SIZEOF))) : (malloc((SIZEOF)))
#endif
#endif
