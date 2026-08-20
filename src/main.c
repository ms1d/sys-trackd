#include <stdio.h>
#include <unistd.h>
#include "cpu_usage.h"
#include "mem_usage.h"
#include "cpu_temp.h"

void cpu_usage_loop(void) {
	for(;;) {
		float res = cpu_usage();
		if ((int)-res == ERR_CPU_USAGE_SAMPLE) continue;
		else if (res > 0) printf("%f\n", res);
		usleep(500000);
	}
}

void mem_usage_loop(void) {
	for (;;) {
		printf("%f\n", mem_usage());
		usleep(100000);
	}
}

void cpu_temp_loop(void) {
    for (;;) {
        printf("%f\n", cpu_temp() / 1000);
		usleep(100000);
    }
}

int main(void) {
	cpu_temp_loop();
	return 1;
}
