#include "cpu_temp.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>


#define ROOT_PATH		"/sys/class/hwmon"
#define PATH_BUFF_SIZE	64
static int temp_fd = -1;

float cpu_temp(void) {
	if (temp_fd < 0) {
		char found_temp = 0;

		char path_to_temp[PATH_BUFF_SIZE] = ROOT_PATH;
		DIR *root_dir = opendir(path_to_temp);
		if (!root_dir) {
			return -ERR_CPU_TEMP_ROOT;
		}

		struct dirent *entry = NULL;
		while ((entry = readdir(root_dir))) {
			const char *name = entry->d_name;
			if (name[0] == '.') continue;

			int res = snprintf(
				path_to_temp,
				sizeof(path_to_temp),
				ROOT_PATH"/%s/name", name
			);

			if (res < 0 || (size_t)res >= sizeof(path_to_temp)) {
				closedir(root_dir);
				return -ERR_CPU_TEMP_SPF;
			}

			int fd = open(path_to_temp, O_RDONLY);
			if (fd < 0) {
				continue;
			}

#define NAME_BUFF_LEN 16
			char name_buffer[NAME_BUFF_LEN] = {0};
			ssize_t read_bytes = 0;
			while (read_bytes < NAME_BUFF_LEN - 1) { // allow 1 byte at the end for null termination
				ssize_t tmp = read(fd, name_buffer + read_bytes, NAME_BUFF_LEN - 1 - read_bytes);
				if (tmp == 0) break;
				if (tmp < 0) {
					if (errno == EINTR) continue;
					close(fd);
					closedir(root_dir);
					return -ERR_CPU_TEMP_READ;
				}
				read_bytes += tmp;
			}
			close(fd);


			if (strcmp(name_buffer, "k10temp\n") == 0 || strcmp(name_buffer, "coretemp\n") == 0 || strcmp(name_buffer, "zenpower\n") == 0 || strcmp(name_buffer, "k8temp\n") == 0) {
				found_temp = 1;
				res = snprintf(
					path_to_temp,
					sizeof(path_to_temp), 
					ROOT_PATH"/%s/temp1_input",
					name
				);
				if (res < 0 || (size_t)res >= sizeof(path_to_temp)) {
					closedir(root_dir);
					return -ERR_CPU_TEMP_SPF;
				}
				break;
			}
#undef NAME_BUFF_LEN
		}

		closedir(root_dir);

		if (!found_temp) return -ERR_CPU_TEMP_NFD;
		temp_fd = open(path_to_temp, O_RDONLY);
		if (temp_fd < 0) return -ERR_CPU_TEMP_NFD;
	}

	char temp_buffer[8];
	int read_bytes = 0;
	while (read_bytes < 8) {
		int tmp = read(temp_fd, temp_buffer + read_bytes, 8 - read_bytes);
		if (tmp == 0) break;
		if (tmp < 0) {
			if (errno == EINTR) continue;
			perror("read");
			close(temp_fd); temp_fd = -1;
			return -ERR_CPU_TEMP_READ;
		}
		read_bytes += tmp;
	}

	if (read_bytes == 0) {
		close(temp_fd);
		temp_fd = -1;
		return -ERR_CPU_TEMP_EMRD;
	}

	int res = 0, power = 1;
	for (int i = read_bytes - 1; i > -1; i--) {
		int dig = temp_buffer[i] - '0';
		if (dig > 9 || dig < 0) continue;
		res += power * dig;
		power *= 10;
	}

#ifdef ENABLE_CACHE
	if (lseek(temp_fd, 0, SEEK_SET) < 0) {
		close(temp_fd);
		temp_fd = -1;
		return -ERR_CPU_TEMP_LSEK;
	}
#else
	close(temp_fd);
	temp_fd = -1;
#endif
	return res;
}
