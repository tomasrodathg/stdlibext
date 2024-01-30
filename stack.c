#include <stdio.h>
#ifndef STACK_H
#include "./stack.h"
#endif

Stack *stk = NULL;

int pushobj(void *obj, void (*cleanup)(void*)) {
	object *o = (object *) malloc(sizeof(object));
	if (o == NULL) {
		fprintf(stderr, "Failed to allocate memory for 'Object'.\n");
		return 1;
	}
	o->obj = obj;
	o->cleanup = cleanup; 

	return push(stk, o);
}

void objfree(object *o) {
	if (o != NULL) {
		if (o->obj != NULL) 
			o->cleanup(o->obj);
		free(o);
	}
}

Stack *createstack(void) {
	Stack *stk = (Stack *)malloc(sizeof(Stack));
	if (stk == NULL) {
		fprintf(stderr, "Failed to allocate memory for the 'Stack'\n");
		return NULL;
	}

	stk->cap = MAX_STACK_SIZE;
	stk->size = 0;

	return stk;
}

// This should change to a growable stack
int push(Stack *stk, object *o) {
	if (stk->size >= stk->cap) {
		fprintf(stderr, "Exceeded max buffer capacity for the stack.\n");
		return 1;
	}

	// add to buffer, increase size
	stk->buffer[stk->size] = o;
	return stk->size++;
}

object *pop(Stack *stk) {
	return (stk->size > 0) ? stk->buffer[--(stk->size)] : NULL;
}

void freestack(Stack *stk) {
	if (stk != NULL) {
		for (size_t i = 0; i < stk->cap; i++) objfree(stk->buffer[i]);
		free(stk);
	}
}
