#include <stdlib.h>
#ifndef __STACK_H__
#define __STACK_H__
#define MAX_STACK_SIZE 100

typedef struct {
	void *obj;
	void (*clean)(void*);
} object;

int pushobj(void *obj, void (*clean)(void*));
void objfree(object *o);

typedef struct {
	object *buffer[MAX_STACK_SIZE];
	size_t cap;
	size_t size;
} Stack;

// This should change to a growable stack
int push(Stack *stk, object *o);
object *pop(Stack *stk);
Stack *createstack(void);
void freestack(Stack *stk);

extern Stack *stk;
#endif
