#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "codes.h"


static int choice = -1;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Ignoring args...\n"); return 1;
	}
	if (strcmp(argv[1], "-u") == 0) {
		choice = OPT_CPU_USAGE;
	} else if (strcmp(argv[1], "-m") == 0) {
		choice = OPT_MEM_USAGE;
	} else if (strcmp(argv[1], "-t") == 0) {
		choice = OPT_CPU_TEMP;
	} else if (strcmp(argv[1], "-f") == 0) {
		choice = OPT_CPU_FREQ;
	} else { printf("Unknown arg\n"); return 2; }

	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));

	addr.sun_family = AF_UNIX;
	addr.sun_path[0] = '\0';
	strcpy(addr.sun_path + 1, "sys_trackd");

	if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("connect"); return 3;
	} 

	{
		int send_bytes = 0;
		while (send_bytes < 4) {
			int tmp = send(fd, (void*)&choice + send_bytes, 4 - send_bytes, 0);
			if (tmp <= 0) { perror("send"); close(fd); return 4; }
			send_bytes += tmp;
		}
	}

	char res[5];
	{
		int read_bytes = 0;
		while (read_bytes < 5) {
			int tmp = read(fd, res + read_bytes, 5 - read_bytes);
			if (tmp <= 0) { perror("read"); close(fd); return 5; }
			read_bytes += tmp;
		}
	}

	if (res[0] != 0) {
		int err = *(int*)(res + 1);
		fprintf(stderr, "Error: %d\n", -err);
		return -err;

	}

	printf("%f\n", *(float*)(res + 1));
	return 0;
}
