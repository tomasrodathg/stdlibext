#include "ctest.h"
#include <errno.h>
#include <stdarg.h>
#ifdef TEST
#define MAX_BUFFER_SIZE 20

typedef void (*testfn)(void);

struct tresult {
	rtype res;
	char *msg;
};

struct tester {
	struct tresult **results;
	size_t rescount;
	size_t failure_count;
	size_t success_count;
	size_t gfactor;
};

static struct tester *t; 
static char *test_name;

void __set_test_name(char *tname) 
{
	test_name = tname;
}

char *__next_test_report(void)
{
	return (t->rescount > 0) ? t->results[--t->rescount]->msg : NULL; 
}

void __print_test_summary(void)
{
	printf("Test results summary:\n\t");
	printf("Successes: %lu | Failures: %lu\n", t->success_count, t->failure_count);
}

void __register_tests(char *mod_name, size_t ntests,...)
{
	testfn test;
	va_list ap;
	printf("Running tests for MODULE `%s`\n", mod_name);
	va_start(ap, ntests);
	while(( ntests-- > 0 && (test = va_arg(ap, testfn)))) test();
}

static void freeres(struct tresult *result)
{
	if(result == NULL) return;
	if(result->msg != NULL) free(result->msg);
	free(result);
}

static void freetester(void)
{
	if(t->results) {
		for(size_t i = 0; i < (t->failure_count + t->success_count); i++) {
			freeres(t->results[i]);
		}
		free(t->results);
	}
	free(t);
}

void __cleanup(void) {
	freetester();
}

int __init_tester(size_t n) {
	if(!(t = (struct tester *) malloc(sizeof(struct tester)))) goto cleanup;
	if(!(t->results = (struct tresult **) malloc(n * sizeof(struct result *)))) goto cleanup;
	for(size_t i = 0; i < n; i++) if(!(t->results[i] = (struct tresult *)malloc(sizeof(struct tresult)))) goto cleanup;
	t->failure_count = 0;
	t->rescount = 0;
	t->success_count = 0;
	t->gfactor = 2;

cleanup:
	if(!t) goto cleanup_tester;
	return 1;

cleanup_tester:
	freetester();
	return 1;
}

static char *build_assertion_msg(void *expected, void *result, rtype msg_type)
{
	const char *test_name_intro = "$|| {TEST `", *success_intro = "` } || <SUCCESS>: Successfully compared values `", *failure_intro = "`} || <FAILURE>: Failed to compare values `", *and_sep = "` and `", *end_sep = "'.";
	size_t failure_msg_len = strlen(test_name_intro) + strlen(failure_intro) + strlen(and_sep) + strlen(test_name) + strlen(expected) + strlen(result) + strlen(end_sep) + 1,
	success_msg_len = strlen(test_name_intro) + strlen(success_intro) + strlen(and_sep) + strlen(test_name) + strlen(expected) + strlen(result) + strlen(end_sep) + 1;

	if(msg_type == SUCCESS) {
		char *succ_msg = (char *) malloc(success_msg_len * sizeof(char));
		if (!succ_msg) return NULL;
		succ_msg = strcat(succ_msg, test_name_intro);
		succ_msg = strcat(succ_msg, test_name);
		succ_msg = strcat(succ_msg, success_intro);
		succ_msg = strcat(succ_msg, expected);
		succ_msg = strcat(succ_msg, and_sep);
		succ_msg = strcat(succ_msg, result);
		succ_msg = strcat(succ_msg, end_sep);
		return succ_msg;
	} else {
		char *failure_msg = (char *) malloc(failure_msg_len * sizeof(char));
		if(!failure_msg) return NULL;
		failure_msg = strcat(failure_msg, test_name_intro);
		failure_msg = strcat(failure_msg, test_name);
		failure_msg = strcat(failure_msg, failure_intro);
		failure_msg = strcat(failure_msg, expected);
		failure_msg = strcat(failure_msg, and_sep);
		failure_msg = strcat(failure_msg, result);
		failure_msg = strcat(failure_msg, end_sep);
		return failure_msg;
	}

	return NULL;
}

struct tresult *createtres(rtype res_type, char *expected, char *result)
{
	struct tresult *tres;
	if(!(tres = (struct tresult *) malloc(sizeof(struct tresult)))) return NULL;
	tres->msg = build_assertion_msg(expected, result, res_type);
	tres->res = res_type;
	return tres;
}

void __assert_true(bool cond, char *expected, char *got)
{
	struct tresult *tres;
	
	if(!(tres = (cond) ?
	createtres(SUCCESS, expected, got):
	createtres(FAILURE, expected, got))) return;

	(tres->res == SUCCESS) ? t->success_count++ : t->failure_count++;
	t->results[t->rescount++] = tres;
}

void __assert_eq_int(int expected, int result)
{
	struct tresult *tres;
	char expectedbuf[10], resultbuf[10];
	sprintf(expectedbuf, "%d", expected);
	sprintf(resultbuf, "%d", result);
	
	if(!(tres = (expected == result) ?
	createtres(SUCCESS, expectedbuf, resultbuf):
	createtres(FAILURE, expectedbuf, resultbuf))) return;

	(tres->res == SUCCESS) ? t->success_count++ : t->failure_count++;
	t->results[t->rescount++] = tres;
}

void __assert_eq_str(char *expected, char *result)
{
	struct tresult *tres;
	
	if(!(tres = (strcmp(expected, result) == 0) ?
	createtres(SUCCESS, expected, result):
	createtres(FAILURE, expected, result))) return;

	(tres->res == SUCCESS) ? t->success_count++ : t->failure_count++;
	t->results[t->rescount++] = tres;
}

__attribute__((constructor(101)))
void setup(void) {
	if(!(__init_tester(20))) exit(1);
	printf("Initialized test runner...\n");
}

int tmain(void)
{
	char *test_report;
	printf("\n");
	while((test_report = __next_test_report())) printf("%s\n", test_report);
	__print_test_summary();
	__cleanup();
	exit(1);
}
#endif
