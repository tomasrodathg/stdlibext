#include <stdlib.h>
#ifndef __VEC_H__
#define __VEC_H__
#define VEC_GENERIC_ERR 1
#define VEC_ALLOC_ERR 2
#define VEC_GROW_ERR 3

typedef struct {
	void *ptr;
	size_t cap;
	size_t len;
	size_t elt_size;
} Vec;

Vec *with_capacity(size_t cap, size_t elt_size);
Vec *from_arr(void *, size_t elt_no, size_t elt_size);
void vfree(Vec *);
void printvec(Vec *);
int grow_vec(size_t);
void clear_vec(Vec *);
int push_to_vec(Vec *, void *);
int push_to_vec_safe(Vec *, void *);
void *pop_from_vec(Vec *);
#endif
