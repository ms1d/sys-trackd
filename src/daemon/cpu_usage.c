#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "cpu_usage.h"
#include "parsers.h"

static int stat_fd = -1;


float cpu_usage(void) {
#define INP_SIZE 200
#define OUT_LEN 8
	static uint64_t old_total = 0, old_idle = 0;
	static char initialised = 0;

	if (stat_fd < 0) {
		stat_fd = open("/proc/stat", O_RDONLY);
		if (stat_fd < 0) { perror("open"); return -ERR_CPU_USAGE_OPEN; }
	}

	char inp[INP_SIZE] = {0};

	int read_bytes = 0;
	while (read_bytes < INP_SIZE - 1) {
		ssize_t tmp = read(stat_fd, inp + read_bytes, INP_SIZE - 1 - read_bytes);

		if (tmp == 0) break;
		if (tmp < 0) {
			if (errno == EINTR) continue;
			perror("read");
			close(stat_fd);
			stat_fd = -1;
			return -ERR_CPU_USAGE_READ;
		}
		
		read_bytes += tmp;
	}


	const char *buf_end = seek_char(inp, read_bytes, '\n');
	if (buf_end == NULL) {
		close(stat_fd);
		stat_fd = -1;
		return -ERR_CPU_USAGE_NEWL;
	}

	const char *buf_start = seek_digit(inp, read_bytes);
	if (buf_start == NULL) {
		close(stat_fd);
		stat_fd = -1;
		return -ERR_CPU_USAGE_STRT;
	}

	uint64_t out[OUT_LEN];

	int out_index = 0;
	while (buf_start != NULL && buf_start < buf_end && out_index < OUT_LEN) {
		const char *num_end = seek_whitespace(buf_start, buf_end - buf_start);
		if (num_end == NULL) num_end = buf_end;
		const char *tmp = num_end;

		uint64_t num = 0, i = 1;
		while (tmp != buf_start) {
			tmp--;
			num += i * (*tmp - '0');
			i *= 10;
		}

		out[out_index++] = num;
		buf_start = seek_not_whitespace(num_end, buf_end - num_end);
	}

	if (out_index != OUT_LEN) {
		close(stat_fd);
		stat_fd = -1;
		return -ERR_CPU_USAGE_OUT;
	}

	uint64_t total = 0, idle = out[3];
	for (int i = 0; i < OUT_LEN; i++) total += out[i];

	if (!initialised) {
		old_total = total; old_idle = idle;
		initialised = 1;
#ifdef ENABLE_CACHE
		if (lseek(stat_fd, 0, SEEK_SET) < 0) {
			perror("lseek");
			close(stat_fd);
			stat_fd = -1;
		}
#else
		close(stat_fd);
		stat_fd = -1;
#endif
		return -ERR_CPU_USAGE_INIT;
	}

	uint64_t diff_total = total - old_total;
    uint64_t diff_idle = idle - old_idle;

	if (diff_total == 0) {
#ifdef ENABLE_CACHE
		if (lseek(stat_fd, 0, SEEK_SET) < 0) {
			perror("lseek");
			close(stat_fd);
			stat_fd = -1;
		}
#else
		close(stat_fd);
		stat_fd = -1;
#endif

		return -ERR_CPU_USAGE_IDLE;
	}

	if (diff_total < diff_idle || old_total > total || old_idle > idle) {
		old_total = total; old_idle = idle;
#ifdef ENABLE_CACHE
		if (lseek(stat_fd, 0, SEEK_SET) < 0) {
			perror("lseek");
			close(stat_fd);
			stat_fd = -1;
		}
#else
		close(stat_fd);
		stat_fd = -1;
#endif

		return -ERR_CPU_USAGE_NVLD;
	}

    old_total = total; old_idle = idle;

#ifdef ENABLE_CACHE
	if (lseek(stat_fd, 0, SEEK_SET) < 0) {
		perror("lseek");
		close(stat_fd);
		stat_fd = -1;
		return -ERR_CPU_USAGE_LSEK;
	}
#else
	close(stat_fd);
	stat_fd = -1;
#endif
	return 1 - (float)diff_idle / (float)diff_total;
#undef OUT_LEN
#undef INP_SIZE
}
