#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#ifndef __PRINT_H__
#include "./print.h"
#endif 

#ifndef STR_H
#include "./str.h"
#endif

#ifndef STACK_H
#include "./stack.h"
#endif

void print_str(Str *str) {
	if (str->type == Safe)
		println("Str:\n\tval: %s\n\tcap: %lu\n\tlen: %lu\n\tstack pos: %lu\n\tgrowth_factor: %lu", str->s, str->cap, str->len, str->spos, str->gfactor);
	else
		println("Str:\n\tval: %s\n\tcap: %lu\n\tlen: %lu\n", str->s, str->cap, str->len);
}

void _print_str(void *ptr) {
	print_str((Str *)ptr);
}

void strfree(Str *str) {
	// check if the `String` pointer has not already been dropped
	if (str != NULL) {
		if (str->s != NULL) free(str->s);
		free(str);
	}
}

// returns a negative value if realloc failed, otherwise it
// returns the new size.
int grow(Str *str, size_t inc) {
	size_t curr_size = str->cap;

	str->s = (char *)realloc(str->s, (curr_size + inc) * sizeof(char));
	if (str->s == NULL) {
		fprintf(stderr, "Failed to reallocate memory for the Str buffer\n");
		return -1;
	}

	str->cap += inc;
	return str->cap;
}

int _grow(void *ptr, size_t inc) {
	return grow((Str *)ptr, inc);
}

int pushstr(Str *str, const char *s) {
	if (str->type == Safe) {
		size_t inlen, new_len;
		inlen = strlen(s);
		if (inlen >= str->cap) {
			new_len = grow(str, (inlen - str->cap) * str->gfactor);
			if(new_len < 0) {
				fprintf(stderr, "Failed to allocate additional space for the Str buffer\n");
				return -1;
			}
			str->gfactor *= 2;
		}
	}

	if(strcpy(str->s,s) == NULL) {
		fprintf(stderr, "Failed to copy string literal '%s' to Str\n", s);
		return -1;
	}

	return 0;
}

int _pushstr(void *ptr, const char *s) {
	return pushstr((Str *)ptr, s);
}

Str *str_from(const char *s, StrType type) {
	size_t slen, spos, gfactor;

	Str *str = (Str *)malloc(sizeof(Str));
	if (str == NULL) {
		fprintf(stderr, "Failed to allocate memory for Str type\n");
		goto exitsafe;
	}

	slen = strlen(s);
	str->s = (char *)malloc((slen + 1) * sizeof(char));
	if(str->s == NULL) {
		fprintf(stderr, "Failed to allocate space for the string literal in Str\n");
		goto exitsafe;
	}

	if(strcpy(str->s,s) == NULL) {
		fprintf(stderr, "Failed to copy string literal '%s' to Str\n", s);
		goto exitsafe;
	}

	str->cap = (slen + 1);
	str->len = (slen + 1);
	str->type = type;
	str->print = _print_str;
	str->grow = _grow;
	str->pushstr = _pushstr;

	spos = gfactor = 0;
	if (type == Safe) {
		if ((spos = pushobj(str, (*strclean)) < 0)) {
			fprintf(stderr, "Failed to push the Str to the stack.\n");
			goto exitsafe;
		}
		gfactor = 2;
	}

	str->gfactor = gfactor;
	str->spos = spos;
	return str;

exitsafe:
	strfree(str);	
	return NULL;
}

void strclean(void *ptr) {
	strfree((Str *) ptr);
}
