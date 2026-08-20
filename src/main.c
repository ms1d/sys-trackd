#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cpu_usage.h"
#include "mem_usage.h"
#include "cpu_temp.h"
#include "cpu_freq.h"

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

void cpu_freq_loop(void) {
	for (;;) {
		printf("%f\n", cpu_freq() / 1000000);
		usleep(100000);
	}
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Ignoring args...\n"); return 1;
	}
	if (strcmp(argv[1], "-u") == 0) {
		cpu_usage_loop(); return 0;
	} else if (strcmp(argv[1], "-m") == 0) {
		mem_usage_loop(); return 0;
	} else if (strcmp(argv[1], "-t") == 0) {
		cpu_temp_loop(); return 0;
	} else if (strcmp(argv[1], "-f") == 0) {
		cpu_freq_loop(); return 0;
	} else { printf("Unknown arg\n"); return 1; }
}
