#include <stdio.h>
#include "bench.h"

int main(void)
{
	__START_BENCH(for_looper);
	for(int i = 0; i < 10000; i++)
		printf("i: %d\n", i);

	__END_BENCH
}
