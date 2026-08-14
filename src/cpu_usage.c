#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cpu_usage.h"

static int stat_fd = -1;
static void close_stat_fd() {
    close(stat_fd);
}

int cpu_usage() {
	// 1 - parse upto newline exclusive (char[1000] as buffer, check for overflow)
	// 2 - repeatedly parse upto each space (10 fields, last 2 are VM specific, not necessary)
	// 3 - sum = total time. 4th field = idle time
	// 4 - usage = change in non-idle / change in total
#define BUF_SIZE 200
	static char buf[BUF_SIZE];

	if (stat_fd < 0) {
		stat_fd = open("/proc/stat", O_RDONLY);
		if (stat_fd < 0) { perror("open"); return ERR_CPU_USAGE_OPEN; }
		atexit(close_stat_fd);
	}
	
	int read_bytes = 0;
	while (read_bytes < BUF_SIZE) {
		int tmp = read(stat_fd, buf + read_bytes, BUF_SIZE - read_bytes);
		if (tmp < 0) {
			perror("read");
			return ERR_CPU_USAGE_READ;
		}
		read_bytes += tmp;
	}

	char *buf_end = buf;

	while ((buf_end - buf < BUF_SIZE) && *(buf_end++) != '\n');
	if (*--buf_end != '\n') return ERR_CPU_USAGE_NEWL;

	if (buf_end + 1 - buf < BUF_SIZE) *(buf_end+1) = '\0';
	printf("%s", buf);

	lseek(stat_fd, 0, SEEK_SET);
	return 0;
#undef BUF_SIZE
}
