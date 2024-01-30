/* Testing framework support for C.
 *
 * Leverages `_Generic` support from C11 in order to allow for
 * calls to assert_eq in a generic fashion. It will likely end up requiring some
 * form of string conversion to be able to deal with complex data structures,
 * but that will potentially be easier by just making the data structure have a
 * `to_string()` function that can then be used with `assert_eq_str()`
 *
 * # Current Macros
 * - assert_t(EXPR,MSG) -> Takes an expression that evaluates to a `int` (i.e. a bool of 1 or 0) and a `msg` 
 *   to be printed in case of assertion failure.
 *
 * - assert_eq(A,B) -> Takes 'A' and 'B' as two generic types of the same kind, and dynamically dispatches the
 * 	 assertion to the right function to test for equality.
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#ifndef __ASSERT__
#define assert_t(EXPR,MSG) assert_true((EXPR),(MSG))
#define assert_eq(A,B) _Generic((A), \
	char*: assert_eq_str, \
	char: assert_eq_char, \
	int: assert_eq_int \
)(A,B)
#endif

void assert_true(int expr, const char *msg) {
	if (expr == 0) {
		fprintf(stderr, "%s\n", msg);
	}
}

void assert_eq_str(char *a, char *b){
	if (strcmp(a, b) != 0) {
		fprintf(stderr, "Failed to compare 'char *' lhs '%s' and rhs '%s'.\n", a, b);
	}
}

void assert_eq_char(char a, char b) {
	if(a != b) {
		fprintf(stderr, "Failed to compare 'char' lhs '%c' and rhs '%c'\n", a, b);
	}
}

void assert_eq_int(int a, int b) {
	if (a != b) {
		fprintf(stderr, "Failed to compare 'int' lhs '%d' and rhs '%d'\n", a, b);
	}
}
