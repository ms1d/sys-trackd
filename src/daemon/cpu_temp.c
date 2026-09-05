#include "cpu_temp.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>


static int temp_fd = -1;
void close_temp_fd(void) {
	close(temp_fd);
}

float cpu_temp(void) {
	if (temp_fd < 0) {
		DIR *root = opendir("/sys/class/hwmon");
		if (root == NULL) return -ERR_CPU_TEMP_ROOT;
		char path_to_temp[64];

		struct dirent *entry;
		while ((entry = readdir(root)) != NULL) {
			if (*entry->d_name == '.') continue;

			char sub_entry_path[64] = "/sys/class/hwmon/";
			memcpy(sub_entry_path + 17, entry->d_name, strlen(entry->d_name));
			int base_len = 17 + strlen(entry->d_name);

			DIR *sub_root = opendir(sub_entry_path);
			if (sub_root == NULL) return -ERR_CPU_TEMP_SBRT;
			
			struct dirent *sub_entry, *temp_entry = NULL;
			while ((sub_entry = readdir(sub_root)) != NULL) {
				const char *name = sub_entry->d_name;
				if (*name == '.') continue;
				
				if (strcmp(name, "name") != 0) continue;

				memcpy(sub_entry_path + base_len, "/", 1);
				memcpy(sub_entry_path + base_len + 1, sub_entry->d_name, strlen(sub_entry->d_name));

				int fd = open(sub_entry_path, O_RDONLY);
				if (fd < 0) return -ERR_CPU_TEMP_OPEN;

				char buffer[32];
				int read_bytes = 0;
				while (read_bytes < 32) {
					int tmp = read(fd, buffer + read_bytes, 32 - read_bytes);
					if (tmp < 0) { perror("read"); return -ERR_CPU_TEMP_READ; }
					if (tmp == 0) break;
					read_bytes += tmp;
				}

				close(fd);

				// Looking for k10temp/zenpower (amd) or coretemp (intel)
				// Lazy method
				if (*buffer == 'k' || *buffer == 'c' || *buffer == 'z') temp_entry = sub_entry;
			}


			if (temp_entry != NULL) {
				memcpy(path_to_temp, sub_entry_path, base_len);
				memcpy(path_to_temp + base_len, "/", 1);
				memcpy(path_to_temp + base_len + 1, "temp1_input", strlen("temp1_input"));
			
				closedir(sub_root);
				break;
			}

			closedir(sub_root);
		}

        closedir(root);
		temp_fd = open(path_to_temp, O_RDONLY);

		if (temp_fd < 0) return -ERR_CPU_TEMP_NFD;
		atexit(close_temp_fd);
	}

	char temp_buffer[8];
	int read_bytes = 0;
	while (read_bytes < 8) {
		int tmp = read(temp_fd, temp_buffer + read_bytes, 8 - read_bytes);
		if (tmp == 0) break;
		if (tmp < 0) { perror("read"); return -ERR_CPU_TEMP_READ; }
		read_bytes += tmp;
	}

	int res = 0, power = 1;
	for (int i = 7; i > -1; i--) {
		int dig = temp_buffer[i] - '0';
		if (dig > 9 || dig < 0) continue;
		res += power * dig;
		power *= 10;
	}

	lseek(temp_fd, 0, SEEK_SET);
	return res;
}
