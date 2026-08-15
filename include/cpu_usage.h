#pragma once

//Error codes for cpu_usage:

// General error code
#define ERR_CPU_USAGE_GEN  1

// open syscall failed
#define ERR_CPU_USAGE_OPEN 2

// read syscall failed
#define ERR_CPU_USAGE_READ 3

// could not find newline character in input (INP_SIZE may be too small)
#define ERR_CPU_USAGE_NEWL 4

// could not find first digit to cut out 'cpu' from /proc/stat's first line
// (INP_SIZE may be too small)
#define ERR_CPU_USAGE_STRT 5

// could not find all values in input buffer (INP_SIZE may be too small, OUT_LEN may be too big)
#define ERR_CPU_USAGE_OUT 6

// need a second sample to record results. call again
#define ERR_CPU_USAGE_SAMPLE -7

float cpu_usage(void);
