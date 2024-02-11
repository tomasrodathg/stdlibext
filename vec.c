#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "./vec.h"

void printerr(int err)
{
	switch (err) {
		case VEC_GENERIC_ERR:
			fprintf(stderr, "Failed with a 'Generic' error. I don't know what this means, but something's wrong with the `Vec`!\n");
			break;
		case VEC_ALLOC_ERR:
			fprintf(stderr, "Failed to allocate memory on creation of the `Vec`. Memory allocation error: errno |%s|.\n", strerror(errno));
			break;
		case VEC_GROW_ERR:
			fprintf(stderr, "Failed to dynamically resize the `Vec`. Memory reallocation error: errno |%s|.\n", strerror(errno));
			break;
	}
}

void printvec(Vec *vec)
{
	printf("vec:\n\tcap: %lu\n\tlen: %lu\n", vec->cap, vec->len);
}

void vfee(Vec *vec)
{
	if(vec != NULL) {
		free(vec->ptr);
		vec->ptr = NULL;
		free(vec);
	}
}

Vec *with_capacity(size_t cap, size_t elt_size)
{
	int err = VEC_GENERIC_ERR;
	Vec *vec = {0};
	if((vec = (Vec *)malloc(sizeof(Vec)))) goto handle_alloc_success;
	err = VEC_ALLOC_ERR;
	goto handle_failure;

handle_alloc_success:
	if(!(vec->ptr = malloc(cap))) goto handle_failure;
	vec->cap = cap;
	vec->len = 0;
	return vec;

handle_failure:
	vfree(vec);
	printerr(err); 
	return NULL;
}

Vec *from_arr(void *arr, size_t no_elts, size_t elt_size)
{
	int err = VEC_ALLOC_ERR;

	Vec *vec;
	if((vec = (Vec *)malloc(sizeof(Vec))) != NULL ) goto mem_alloc_arr;
	goto handle_failure;

mem_alloc_arr:
	if((vec->ptr = malloc(no_elts * elt_size)) != NULL) goto add_arr_elts;
	goto handle_failure;

add_arr_elts:
	memcpy(vec->ptr, arr, no_elts * elt_size);
	vec->len = vec->cap = no_elts;
	vec->elt_size = elt_size;
	return vec;

handle_failure:
	vfree(vec);
	printerr(err);
	return NULL;
}

int grow_vec(size_t inc)
{
	return 0;
}

void clear_vec(Vec *vec)
{

}

int push_to_vec(Vec *vec, void *elt)
{
	return 0;
}

int push_to_vec_safe(Vec *vec, void *elt)
{
	return 0;
}

void *pop_from_vec(Vec *vec)
{
	return NULL;
}
