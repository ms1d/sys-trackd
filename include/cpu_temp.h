#pragma once

// Error codes for cpu_temp:

// General error code
#define ERR_CPU_TEMP_GEN 1

// Could not open the "root" directory (/sys/class/hwmon/)
#define ERR_CPU_TEMP_ROOT 2

// Could not open the "sub_root" directory (/sys/class/hwmon/hwmon*)
#define ERR_CPU_TEMP_SBRT 3

// open syscall failed to obtain device name
#define ERR_CPU_TEMP_OPEN 4

// read syscall failed
#define ERR_CPU_TEMP_READ 5

// Could not find device (code is not portable, expected on different architectures)
#define ERR_CPU_TEMP_NFD 6

// WARNING - the parsing and directory exploration in this function
// are quite flimsy. Do not expect this to be portable everywhere.
// I tested it on my own hardware (lenovo ideapad 5 pro 14ahp9, ryzen 7 8845HS)
// and it works fine. Send patches if you want
float cpu_temp(void);
