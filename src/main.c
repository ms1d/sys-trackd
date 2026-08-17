#include <stdio.h>
#include <unistd.h>
#include "cpu_usage.h"
#include "mem_usage.h"


int main(void) {
	for (;;) {
		printf("%f\n", mem_usage());
	}
	for(;;) {
		float res = cpu_usage();
		if ((int)-res == ERR_CPU_USAGE_SAMPLE) continue;
		else if (res > 0) printf("%f\n", res);
		usleep(500000);
	}

	return 1;
}
