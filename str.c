#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifndef __PRINT_H__
#include "./print.h"
#endif 
#ifndef __STR_H__
#include "./str.h"
#endif
#ifndef __STACK_H__
#include "./stack.h"
#endif

void print_string_err(int err) 
{
	switch (err) {
		case STR_GENERIC_ERR: 
			fprintf(stderr, "Failed with a 'Generic Str Error'. This is an error I did not expect!\n");
			break;
		case STR_ALLOC_ERR:
			fprintf(stderr, "Error due to a failed memory allocation: errno %d.\n", errno);
			break;
		case STR_GROW_ERR:
			fprintf(stderr, "Error when attempting to grow the 'str' via realloc: errno %d.\n", errno);
			break;
		case STR_CPY_ERR:
			fprintf(stderr, "Failed to copy a value to the 'str' memory location.\n");
			break;
		}
}

void print_string(string *str) 
{
	(str->type == Safe) 
	? println("string:\n\tval: %s\n\tcap: %lu\n\tlen: %lu\n\tstack pos: %lu\n\tgrowth_factor: %lu", empty_or_val(str->s), str->cap, str->len, str->spos, str->gfactor)
	: println("string:\n\tval: %s\n\tcap: %lu\n\tlen: %lu\n", str->s, str->cap, str->len);
}

void _print_string(void *ptr) 
{
	print_string((string *)ptr);
}

void stringfree(string *str) 
{
	// check if the `String` pointer has not already been dropped
	if (str != NULL) {
		if (str->s != NULL) free(str->s);
		free(str);
	}
}

// returns a negative value if realloc failed, otherwise it
// returns the new size.
int grow(string *str, size_t inc) 
{
	size_t curr_size = str->cap;

	str->s = (char *)realloc(str->s, (curr_size + inc) * sizeof(char));
	if (str->s == NULL) {
		fprintf(stderr, "Failed to reallocate memory for the Str buffer\n");
		return -1;
	}

	str->cap += inc;
	return str->cap;
}

int _grow(void *ptr, size_t inc) 
{
	return grow((string *)ptr, inc);
}

int grow_safe_string(string *str, size_t input_len) 
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
void clear(string *str) 
{
	// zero the value at the memory address
	bzero(str->s, str->len);

	// set the length back to zero, 'capacity remains unchanged'
	str->len = 0;
}

int pushstring(string *str, const char *s) 
{
	// set an initial err value to STR_GENERIC_ERR
	// in case something goes wrong unexpectedly
	int err = STR_GENERIC_ERR;
	size_t new_cap, prev_cap, input_len;

	// initialize the previous capacity and new capacity 
	// as the current string capacity
	prev_cap = new_cap = str->cap;
	input_len = strlen(s);

	// if the `string` is of type `Safe`, call 'grow' to dynamically
	// grow the capacity if needed.
	if (str->type == Safe) new_cap = grow_safe_string(str, input_len);
	
	// if the new capacity is greater or equal than the previous capacity
	// goto `handle_successful_grow` to copy the new string to buffer
	if (new_cap >= prev_cap) goto handle_successful_grow;
	err = errno;
	goto handle_failed_push;

// this copies the input into the memory of the string object
handle_successful_grow:
	if (strcpy(str->s,s) != NULL) goto handle_successful_cpy;
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
	print_string_err(err);
	return err;
}

int _pushstring(void *ptr, const char *s) 
{
	return pushstring((string *)ptr, s);
}

string *string_from(const char *s, str_type type) 
{
	int err = STR_GENERIC_ERR;
	string *str = {0};
	size_t slen, spos, gfactor;

	if ((str = (string *)malloc(sizeof(string))) != NULL) goto handle_buffer_alloc;
	err = errno;
	goto handle_failure;

handle_buffer_alloc:
	slen = strlen(s);
	if ((str->s = (char *)malloc((slen + 1) * sizeof(char))) != NULL) goto handle_strcpy;
	err = errno;
	goto handle_failure;

handle_strcpy:
	if (strcpy(str->s,s) != NULL) goto handle_safe_str; 
	err = STR_CPY_ERR;
	goto handle_failure;

handle_safe_str:
	spos = gfactor = 0;
	if (type != Safe) goto handle_assign_values;
	if ((spos = pushobj(str, (*stringclean)) < 0)) goto handle_failure;
	gfactor = 2;

handle_assign_values:
	str->cap = (slen + 1);
	str->len = (slen + 1);
	str->type = type;
	str->print = _print_string;
	str->grow = _grow;
	str->pushstr = _pushstring;
	str->gfactor = gfactor;
	str->spos = spos;
	return str;

handle_failure:
	print_string_err(err);
	stringfree(str);	
	return NULL;
}

void stringclean(void *ptr) 
{
	stringfree((string *) ptr);
}
