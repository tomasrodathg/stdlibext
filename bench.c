#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "ctest.h"

struct timer {
	clock_t start_time;
	clock_t end_time;
	const char *fname;
	double ipc;
};

struct timer *initt(void)
{
	struct timer *t;
	if(!(t = (struct timer*) malloc(sizeof(struct timer)))) return NULL;
	return t;
}

void freet(struct timer **t)
{
	if(*t != NULL) free(*t), (*t) = NULL;
}

void start(struct timer *t, const char *fname)
{
	t->start_time = clock();
	t->fname = fname;
}

void end(struct timer *t)
{
	t->end_time = clock();
}

void calcipc(struct timer *t)
{
	t->ipc = (double)(t->end_time - t->start_time) / CLOCKS_PER_SEC;
}

void printipc(struct timer *t)
{
	printf("[%s] time took by CPU: %fs\n", t->fname, t->ipc);
}

__TEST(test_end,
		struct timer t = {0};
		assert_true(t.end_time == 0, "t.end_time", "STARTS ZEROED");
		end(&t);
		assert_true(t.end_time != 0, "t.end_time", "NO LONGER ZEROED");
)

__TEST(test_start,
		struct timer t = {0};
		assert_true(t.start_time == 0, "t.start_time", "STARTS ZEROED");
		start(&t);
		assert_true(t.start_time != 0,"t.start_time", "NO LONGER ZEROED");
)


// TODO: flaky test because no work is done. May be worth introducing 'sleep'
__TEST(test_calc_ipc,
		struct timer t = {0};
		start(&t);
		end(&t);
		assert_true(t.ipc == 0.0, "t.ipc", "STARTS ZEROED");
		calcipc(&t);
		assert_true(t.ipc != 0.0, "t.ipc", "NO LONGER ZEROED");
)

__TEST_MOD(bench, 3, test_start, test_end, test_calc_ipc)
