#pragma once

// Error codes for mem_usage:

// General error code
#define ERR_MEM_USAGE_GEN 1

// open syscall failed
#define ERR_MEM_USAGE_OPEN 2

// read syscall failed
#define ERR_MEM_USAGE_READ 3

// could not find newline character in input (INP_SIZE may be too small)
#define ERR_MEM_USAGE_NEWL 4

// could not find digit in input (INP_SIZE may be too small)
#define ERR_MEM_USAGE_DIG 5


// Returns number of kBs memory used
float mem_usage(void);

// Orchestrates mem_usage to return a percentage
float mem_usage_rel(void);
