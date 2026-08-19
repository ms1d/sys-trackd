#pragma once

// Error codes for cpu_temp:

// General error code
#define ERR_CPU_TEMP_GEN 1

// Could not find/open the hwmon file
#define ERR_CPU_TEMP_OPEN 2

// read syscall failed
#define ERR_CPU_TEMP_READ 3

// WARNING - the parsing and directory exploration in this function
// are quite flimsy. Do not expect this to be portable everywhere.
// I tested it on my own hardware (lenovo ideapad 5 pro 14ahp9, ryzen 7 8845HS)
// and it works fine. Send patches if you want
float cpu_temp(void);
