#include "cpu_freq.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_CORES 32

static int freq_fds[MAX_CORES];
static int freq_fds_len = 0;
void close_freq_fd(void) {
	for (int i = 0; i < freq_fds_len; i++) close(freq_fds[i]);
}

float cpu_freq(void) {
	if (freq_fds_len == 0) {
		DIR *root = opendir("/sys/devices/system/cpu");
		if (!root) return -ERR_CPU_FREQ_ROOT;

		struct dirent *entry;
		while ((entry = readdir(root)) != NULL) {
			char *name = entry->d_name;
			if (name[3] - '0' < 0 || name[3] - '9' > 9) continue;

			char path[64] = "/sys/devices/system/cpu/";
            memcpy(path + 24, name, strlen(name));

			DIR *sub_root = opendir(path);
			if (!sub_root) return -ERR_CPU_FREQ_SBRT;

			struct dirent *sub_entry;
			while ((sub_entry = readdir(sub_root)) != NULL) {
				if (strcmp(sub_entry->d_name, "cpufreq") != 0) continue;
				
				memcpy(path + 24 + strlen(name), "/", 1);
				memcpy(path + 25 + strlen(name), sub_entry->d_name, strlen(sub_entry->d_name));
				memcpy(path + 25 + strlen(name) + strlen(sub_entry->d_name), "\0", 1);
				DIR *freq_root = opendir(path);
				if (!freq_root) return -ERR_CPU_FREQ_FRRT;

				struct dirent *freq_entry;
				while ((freq_entry = readdir(freq_root)) != NULL) {
					if (strcmp(freq_entry->d_name, "scaling_cur_freq") != 0) continue;

					memcpy(path + 25 + strlen(name) + strlen(sub_entry->d_name), "/", 1);
					memcpy(path + 26 + strlen(name) + strlen(sub_entry->d_name), freq_entry->d_name, strlen(freq_entry->d_name));
					memcpy(path + 27 + strlen(name) + strlen(sub_entry->d_name) + strlen(freq_entry->d_name) + 16, "\0", 1);

					int fd = open(path, O_RDONLY);
					if (fd < 0) { perror("open"); return -ERR_CPU_FREQ_OPEN; }
					if (freq_fds_len == MAX_CORES) { close(fd); continue; }
					freq_fds[freq_fds_len++] = fd;
				}
			}
		}

		if (freq_fds_len == 0) {
			return -ERR_CPU_FREQ_NFD;
		}
	}

	float avg = 0;
	for (int i = 0; i < freq_fds_len; i++) {
		int read_bytes = 0;
		char buffer[16];
		while (read_bytes < 16) {
			int tmp = read(freq_fds[i], buffer + read_bytes, 16 - read_bytes);
			if (tmp < 0) { perror("read"); return -ERR_CPU_FREQ_READ; }
			if (tmp == 0) break;
			read_bytes += tmp;
		}

		int power = 1, num = 0;
		for (int j = 15; j > -1; j--) {
			int dig = buffer[j] - '0';
			if (dig < 0 || dig > 9) continue;
			num += power * dig;
			power *= 10;
		}

		avg = (avg * i + (float)num) / (i+1);
		lseek(freq_fds[i], 0, SEEK_SET);
	}

    return avg;
}
