#include "mem_usage.h"
#include "parsers.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static int stat_fd = -1;
static void close_stat_fd(void) {
	close(stat_fd);
}


float mem_usage(void) {
#define INP_SIZE 200
	static char inp[INP_SIZE];
	if (stat_fd < 0) {
		stat_fd = open("/proc/meminfo", O_RDONLY);
		if (stat_fd < 0) { perror("open"); return -ERR_MEM_USAGE_OPEN; }
		atexit(close_stat_fd);
	}

	int read_bytes = 0;
	while (read_bytes < INP_SIZE) {
		int tmp = read(stat_fd, inp + read_bytes, INP_SIZE - read_bytes);
		if (tmp < 0) { perror("read"); return -ERR_MEM_USAGE_READ; }
		read_bytes += tmp;
	}

	char *tot_start = inp;
	const char *tot_end = seek_char(tot_start, INP_SIZE, '\n');
	if (tot_end == NULL) return -ERR_MEM_USAGE_NEWL;
	
	// MemFree is not necessary, but put here for explicitness
	// Compiler will probably optimise this out
	const char *free_start = tot_end + 1;
	const char *free_end = seek_char(free_start, INP_SIZE - (free_start - inp), '\n');
	if (free_end == NULL) return -ERR_MEM_USAGE_NEWL;

	const char *avail_start = free_end + 1;
	const char *avail_end = seek_char(avail_start, INP_SIZE - (avail_start - inp), '\n');
	if (avail_end == NULL) return -ERR_MEM_USAGE_NEWL;

	const char *starts[2] = { tot_start, avail_start };
	uint64_t nums[2] = { 0, 0 };

	for (int i = 0; i < 2; i++) {
		const char *first_dig = seek_digit(starts[i], INP_SIZE - (starts[i] - inp));
		if (first_dig == NULL) return -ERR_MEM_USAGE_DIG;
		const char *last_dig = seek_char(starts[i], INP_SIZE - (starts[i] - inp), 'k');
		if (last_dig == NULL) return -ERR_MEM_USAGE_DIG;
		last_dig -= 2;

		int j = 1;
		while (first_dig <= last_dig) {
			nums[i] += (*last_dig - '0') * j;
			last_dig--;
			j *= 10;
		}
	}

	lseek(stat_fd, 0, SEEK_SET);
	uint64_t used = nums[0] - nums[1];
    return (float)used / (float)nums[0];
#undef INP_SIZE
}
