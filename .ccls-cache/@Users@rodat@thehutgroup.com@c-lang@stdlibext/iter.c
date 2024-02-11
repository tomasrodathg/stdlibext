#include "iter.h"
#include <string.h>
#include <ctype.h>
#include "ctest.h"

/* # DESCRIPTION
 *
 * `peek` is a utility method that allows for
 * a value on iteration to be 'peeked' - i.e. the value is
 * evaluated without moving the iterator forward. It does
 * so via pointer arithmetic without updating the current
 * position of the pointer.
 *
 * This is useful in scenarios where you may want to check the
 * next value of an iteration without 'consuming' it
 *
 * # RETURN
 * A `reference` to the next character in the iteration.
 *
 *
 * # EXAMPLE 
 * ```c
 * int c;
 * char *s = "abcd";
 * 
 * // while the string is not at null termination '\0'
 * // move forward. 
 * // if the next value is 'c' then break.
 * while((c = *s++)) if(peek(s) == 'c') break;
 * 
 * // because we peeked, the iteration will have stopped
 * // at 'b' rather than 'c'.
 * assert_eq_int('b', *s);
 * ```
 */
__attribute__((always_inline))
int peek(char *s)
{
	return *(s + 1);
}

/* # DESCRIPTION
 *
 * `next` is a utility method that moves a pointer
 * forward by one.
 * 
 * This is used for when you want to consume an element and
 * move forward.
 *
 * # RETURN
 * 
 * The next character in iteration, consumed.
 *
 * # EXAMPLE
 * ```c
 * int c;
 * char *s = "abcd";
 * 
 * // while the string is not at null termination '\0'
 * // move forward. 
 * // if the current value is 'c' then break.
 * while((c = next(&s))) if(c == 'c') break;
 * assert_eq_int('c', c);
 * ```
 */
__attribute__((always_inline))
int next(char **s)
{
	(*s)++;
	return **s; 
}


/* # DESCRIPTION
 *
 * `skip_until_eq` moves the pointer of the 'haystack'
 * forward until it matches a given `needle`.
 *
 * # RETURN
 * 
 * 0 if successful, an error code otherwise. The haystack pointer is updated to point at the needle if found. Reset to start if  not.
 *
 * # ERRORS
 *
 * - ENULLHAYSTACK - indicates the haystack was null or empty
 * - EEMPTYNEEDLE - indicates that the needle is empty or null
 * - ENOMATCHFOUND - indicates that not match for that needle was found in the haystack.
 */
int __skip_until_eq(char **haystack, char* needle)
{
	char *start;
	size_t needlelen, haystacklen;

	start = *haystack;
	if(!haystack || !(*haystack)) return ENULLHAYSTACK;
	if(!needle) return EEMPTYNEEDLE;
	needlelen = strlen(needle), haystacklen = strlen(*haystack);
	while((*(*haystack)) && haystacklen >= needlelen) {
		if(strncmp((*haystack), needle, needlelen) == 0) return 0;
		else ((*haystack)++,haystacklen--);
	}
	*haystack = start;
	return ENOMATCHFOUND;
}

__TEST(test_skip_until_eq_no_match,
		char p[] = "<html><meta text='123'/><meta text='233232'/><body></body></html>";
		char *ptr = p;
		char *needle = "<p>";
		int res = __skip_until_eq(&ptr, needle);
		assert_eq_int(ENOMATCHFOUND, res);
		assert_eq_str(p,ptr);
)

__TEST(test_skip_until_eq,
		char p[] = "<html><meta text='123'/><meta text='233232'/><body></body></html>";
		char *ptr = p;
		char *needle = "<body>";
		int res = __skip_until_eq(&ptr, needle);
		assert_eq_int(0, res);
		assert_eq_str("<body></body></html>",ptr);
)

__TEST(test_next,
	int c;
	char *w = "abcd";
	while((c = next(&w)))
		if(c == 'c') break;

	assert_eq_int('c', *w);
)

__TEST(test_peek,
		int c;
		char *w = "abcd";
		while((c = next(&w)))
			if(peek(w) == 'c') break;

		assert_eq_int('b',*w);
)

__TEST_MOD(iter,4,test_next,test_skip_until_eq_no_match,test_peek,test_skip_until_eq)
