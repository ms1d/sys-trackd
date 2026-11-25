#pragma once

// Error codes for cpu_temp:

// General error code
#define ERR_CPU_TEMP_GEN 1

// Could not open the "root" directory (/sys/class/hwmon/)
#define ERR_CPU_TEMP_ROOT 2

// lseek failed when resetting cached temp_fd
#define ERR_CPU_TEMP_LSEK 3

// read 0 bytes from temp_fd
#define ERR_CPU_TEMP_EMRD 4

// read syscall failed
#define ERR_CPU_TEMP_READ 5

// Could not find device (code is not portable, expected on different architectures)
#define ERR_CPU_TEMP_NFD 6

// snprintf truncated output (file paths are longer than anticipated)
#define ERR_CPU_TEMP_SPF 7

// WARNING - the parsing and directory exploration in this function
// have not been tested across many machines. Do not expect this to
// be portable everywhere. I tested it on my own hardware (lenovo
// ideapad 5 pro 14ahp9, ryzen 7 8845HS) and it works fine.

// Send patches to improve coverage if you want
float cpu_temp(void);
