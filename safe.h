 /* A 'Safe Wrapper' around some of C's main features.
 *
 * The goal is to minimize unsafe behaviour while also ensure that C's
 * rawness, low overhead runtime and minimalistic approach - as well as
 * the power of manual management of resources - are preserved
 */
#include <stdlib.h>
#include <stdio.h>
#ifndef __ASSERTION_H__
#include "./assertion.h"
#endif

#ifndef __SMALLOC__
#define smalloc(SIZEOF) safe_malloc((SIZEOF)) 
#endif

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	assert_t(ptr != NULL, "Failed to allocate memory to pointer");
	return ptr;
}
