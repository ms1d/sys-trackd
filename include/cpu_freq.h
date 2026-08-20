#pragma once

// Error codes for cpu_freq

// General error code
#define ERR_CPU_FREQ_GEN 1

// Could not open "root" directory (/sys/devices/system/cpu/)
#define ERR_CPU_FREQ_ROOT 2

// Could not open "sub_root" directory (/sys/devices/system/cpu/cpu*)
#define ERR_CPU_FREQ_SBRT 3

// Could not find "freq_root" directory (/sys/devices/system/cpu/cpu*/cpufreq/)
#define ERR_CPU_FREQ_FRRT 4

// open syscall failed
#define ERR_CPU_FREQ_OPEN 5

// read syscall failed
#define ERR_CPU_FREQ_READ 6

// No cpufreq files found
#define ERR_CPU_FREQ_NFD 7

// Explores /sys/devices/system/cpu/cpu*/cpufreq/ for cpu frequencies
// Upto 32 logical cores (further cores are ignored)
float cpu_freq(void);
