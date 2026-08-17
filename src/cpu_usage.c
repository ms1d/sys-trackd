#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpu_usage.h"
#include "parsers.h"

static int stat_fd = -1;
static void close_stat_fd(void) {
    close(stat_fd);
}


float cpu_usage(void) {
	// 1 - parse upto newline exclusive (char[1000] as buffer, check for overflow)
	// 2 - repeatedly parse upto each space (10 fields, last 2 are VM specific, not necessary)
	// 3 - sum = total time. 4th field = idle time
	// 4 - usage = change in non-idle / change in total
#define INP_SIZE 200
#define OUT_LEN 8
	static char inp[INP_SIZE];
	static uint64_t out[8], old_total = 0, old_idle = 0;

	if (stat_fd < 0) {
		stat_fd = open("/proc/stat", O_RDONLY);
		if (stat_fd < 0) { perror("open"); return -ERR_CPU_USAGE_OPEN; }
		atexit(close_stat_fd);
	}
	
	int read_bytes = 0;
	while (read_bytes < INP_SIZE) {
		int tmp = read(stat_fd, inp + read_bytes, INP_SIZE - read_bytes);
		if (tmp < 0) {
			perror("read");
			return -ERR_CPU_USAGE_READ;
		}
		read_bytes += tmp;
	}


	const char *buf_end = seek_char(inp, INP_SIZE, '\n');
	if (buf_end == NULL) return -ERR_CPU_USAGE_NEWL;

	const char *buf_start = seek_digit(inp, INP_SIZE);
	if (buf_start == NULL) return -ERR_CPU_USAGE_STRT;

	int out_index = 0;
	while (buf_start < buf_end && out_index < OUT_LEN) {
		const char *num_end = seek_char(buf_start, (int)(buf_end - buf_start), ' ');
		if (num_end == NULL) num_end = buf_end;
		const char *tmp = num_end;

		uint64_t num = 0, i = 1;
		while (tmp != buf_start) {
			tmp--;
			num += i * (*tmp - '0');
			i *= 10;
		}

		out[out_index++] = num;
		buf_start = num_end + 1;
	}

	if (out_index != OUT_LEN) return -ERR_CPU_USAGE_OUT;

	uint64_t total = 0, idle = out[3];
	for (int i = 0; i < OUT_LEN; i++) total += out[i];

	if (old_total == 0 || old_idle == 0) {
		old_total = total; old_idle = idle;
		return -ERR_CPU_USAGE_SAMPLE;
	}

	uint64_t diff_total = total - old_total;
    uint64_t diff_idle = idle - old_idle;
    old_total = total; old_idle = idle;

	lseek(stat_fd, 0, SEEK_SET);
	return 1 - (float)diff_idle / (float)diff_total;

#undef OUT_LEN
#undef INP_SIZE
}
