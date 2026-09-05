#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "cpu_usage.h"
#include "mem_usage.h"
#include "cpu_temp.h"
#include "cpu_freq.h"
#include "codes.h"



int send_dat(int fd, const char* dat, const int len) {
	int send_bytes = 0, res = 0;
	while (send_bytes < len) {
		int tmp = send(fd, dat + send_bytes, len - send_bytes, 0);
		if (tmp <= 0) { perror("send"); res = -1; break; }
		send_bytes += tmp;
	}

	close(fd);
	return res;
}

int main(void) {
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));

	addr.sun_family = AF_UNIX;
	addr.sun_path[0] = '\0';
	strcpy(addr.sun_path + 1, "sys_trackd");

	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("bind"); return 1;
	}

	if (listen(fd, 5) == -1) {
		perror("listen"); return 1;
	}

	while(1) {
		int client_fd = accept(fd, NULL, NULL);
		if (client_fd < 0) break;

		char buf[4];

		{
			int read_bytes = 0;
			while (read_bytes < 4) {
				int tmp = read(client_fd, buf + read_bytes, 4 - read_bytes);
				if (tmp == 0) break;
				if (tmp < 0) {
					int err = -ERR_READ;
					char buf[5]; buf[0] = 1; memcpy(buf + 1, &err, 4);
					if (send_dat(client_fd, buf, 5) < 0) break;
					continue;
				}
				read_bytes += tmp;
			}
		}

		char out[5]; float res;
		switch (*(int*)buf) {
			case OPT_CPU_USAGE:
				res = cpu_usage(); break;
			case OPT_MEM_USAGE:
				res = mem_usage(); break;
			case OPT_CPU_TEMP:
				res = cpu_temp(); break;
			case OPT_CPU_FREQ:
				res = cpu_freq(); break;
			default:
				res = -ERR_BAD_OPT; break;
		}

		if (res < 0) { 
			out[0] = 1;
			int res_int = (int)res;
			memcpy(out + 1, &res_int, 4);
			fprintf(stderr, "err byte set to %d\n", *(int*)(out+1));
		}
		else {
			out[0] = 0;
			memcpy(out + 1, &res, 4);
		}
		
		if (send_dat(client_fd, out, 5) < 0) { perror("send"); break; }
	}

	close(fd);
	return 1;
}
