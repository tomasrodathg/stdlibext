#ifndef __BENCH_H
#define __BENCH_H
struct timer;
struct timer *initt(void);
void freet(struct timer **);
void start(struct timer *, const char *);
void end(struct timer *);
void calcipc(struct timer *);
void printipc(struct timer *);
#ifdef BENCH
#define __START_BENCH(FNAME) \
{ \
	struct timer *t = initt();\
	start(t, #FNAME); 
#define __END_BENCH end(t); \
	calcipc(t); \
	printipc(t); \
	freet(&t); \
}
#else
#define __START_BENCH(FNAME)
#define __END_BENCH
#endif
#endif
