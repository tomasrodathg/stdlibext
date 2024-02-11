#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#ifndef __PRINT_H__
#include "./print.h"
#endif 
#include "./str.h"
#include "safe.h"
#include "./stack.h"

void print_str_err(int err) 
{
	switch (err) {
		case STR_GENERIC_ERR: 
			fprintf(stderr, "Failed with a 'Generic Str Error'. This is an error I did not expect!\n");
			break;
		case STR_ALLOC_ERR:
			fprintf(stderr, "Error due to a failed memory allocation: errno |%s|.\n", strerror(errno));
			break;
		case STR_GROW_ERR:
			fprintf(stderr, "Error when attempting to grow the 'str' via realloc: errno |%s|.\n", strerror(errno));
			break;
		case STR_CPY_ERR:
			fprintf(stderr, "Failed to copy a value to the 'str' memory location.\n");
			break;
		}
}

void print_str(Str *str) 
{
	println("str:\n\tval: %s\n\tcap: %lu\n\tlen: %lu\n\tgrowth_factor: %lu", empty_or_val(str->ptr), str->cap, str->len, str->gfactor);
}

void _print_str(void *ptr) 
{
	print_str((Str *)ptr);
}

void str_free(Str *str) 
{
	// check if the `String` pointer has not already been dropped
	if (str != NULL) {
		if (str->ptr != NULL) free(str->ptr);
		free(str);
	}
}

// returns a negative value if realloc failed, otherwise it
// returns the new size.
int grow(Str *str, size_t inc) 
{
	char *tmp = {0};
	if((tmp = (char *)realloc(str->ptr, (str->cap + inc) * sizeof(char))) != NULL) goto increase_capacity;
	free(str->ptr);
	return STR_GROW_ERR;

increase_capacity:
	str->ptr = tmp;
	str->cap += inc;
	return str->cap;
}

int grow_safe_str(Str *str, size_t input_len) 
{
	return (input_len >= str->cap) 
	? grow(str, (input_len - str->cap) * str->gfactor) 
	: str->cap;
}

/* Clears the string without deallocating memory.
 * Simply put, it zeroes the initialized memory so it no longer 
 * holds any actual "text"
 *
 * # linux man <string.h> (as an alternative for `memset`):
 * void bzero(void *s, size_t len);
 */
void clear(Str *str) 
{
	bzero(str->ptr, str->len);
	str->len = 0;
}

int push_str_safe(Str *str, const char *s)
{
	// set an initial err value to STR_GENERIC_ERR
	// in case something goes wrong unexpectedly
	int err = STR_GENERIC_ERR;
	size_t new_cap, prev_cap, input_len;

	// initialize the previous capacity and new capacity 
	// as the current string capacity
	prev_cap = new_cap = str->cap;
	input_len = strlen(s);

	// call 'grow' to dynamically grow the capacity if needed.
	new_cap = grow_safe_str(str, input_len);
	
	// if the new capacity is greater or equal than the previous capacity
	// goto `handle_successful_grow` to copy the new string to buffer
	if (new_cap >= prev_cap) goto handle_successful_grow;
	err = errno;
	goto handle_failed_push;

// this copies the input into the memory of the string object
handle_successful_grow:
	if (strcpy(str->ptr,s) != NULL) goto handle_successful_cpy;
	err = STR_CPY_ERR;
	goto handle_failed_push;

// this increases the capacity of the string if there's been realloc;
// it also sets the length to the input length + 1 (to cater for '\0')
handle_successful_cpy:
	str->len = ++input_len;
	str->cap = new_cap;
	return 0;

// in case of failure, return an error value (int) and 
// print the textual representation of the error.
handle_failed_push:
	print_str_err(err);
	return err;
}

void clean(void *ptr) 
{
	str_free((Str *) ptr);
}

Str *str_from(const char *s) 
{
	int err = STR_GENERIC_ERR;
	Str *str = {0};
	size_t slen;

	if ((str = smalloc(sizeof(Str))) != NULL) goto handle_buffer_alloc;
	err = errno;
	goto handle_failure;

handle_buffer_alloc:
	slen = strlen(s);
	if ((str->ptr = smalloc((slen + 1) * sizeof(char))) != NULL) goto handle_strcpy;
	err = errno;
	goto handle_failure;

handle_strcpy:
	if (strcpy(str->ptr,s) != NULL) goto handle_safe_str; 
	err = STR_CPY_ERR;
	goto handle_failure;

handle_safe_str:
	str->cap = (slen + 1);
	str->len = (slen + 1);
	str->print = _print_str;
	str->gfactor = 2;
	return str;

handle_failure:
	print_str_err(err);
	str_free(str);	
	return NULL;
}
