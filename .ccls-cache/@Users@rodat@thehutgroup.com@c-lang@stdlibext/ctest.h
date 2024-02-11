#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifndef __CTEST_H
#define __CTEST_H
#ifndef __CTEST_ERR
#define __CTEST_ERR
#define ETFNBUFALLOC 1 // failed to allocate memory to test function buffer
#define ETFNBUFGROW 2 // failed to dynamically grow test function buffer
#endif
#ifndef __ASSERT_MOD
#define __ASSERT_MOD
void __assert_eq_str(char *, char *);
void __assert_eq_int(int, int);
void __assert_true(bool, char*, char *);
#define assert_true(EXPR,A,B) (__assert_true((EXPR),A,B))
#define assert_eq_str(A,B) (__assert_eq_str(A,B));
#define assert_eq_int(A,B) (__assert_eq_int(A,B));
#endif
typedef enum {
	SUCCESS,
	FAILURE,
} rtype;
int __init_tester(size_t n);
void __cleanup(void);
char *__next_test_report(void);
void __print_test_summary(void);
void __set_test_name(char *);
void __register_tests(char *, size_t,...);
#define ARRAY_WITH(...) {__VA_ARGS__};
#ifdef TEST
#define __TEST(FN,BODY) void FN(void) \
{ \
	__set_test_name(#FN); \
	BODY; \
}	
#define __TEST_MOD(MOD_NAME,NTESTS,...) \
	__attribute__((constructor(102))) \
	void __init_test_##MOD_NAME(void) \
	{ \
		(__register_tests(#MOD_NAME,NTESTS __VA_OPT__(,) __VA_ARGS__)); \
	} 
#else
#define __TEST(FN,BODY)
#define __TEST_MOD(MOD_NAME,NTESTS,...)
#endif
#endif
