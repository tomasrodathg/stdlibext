#include <stdlib.h>

#define MAX_STACK_SIZE 100

typedef struct {
	void * obj;
	void (*cleanup)(void*);
} object;

int pushobj(void *obj, void (*cleanup)(void*));
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
