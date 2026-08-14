# gaming-overlay

## About

Little C gaming overlay daemon. Collects stats from machine, collates it to JSON.
Will personally use with waybar but can ideally be reused anywhere.

## Primitves

- **CPU (overall)**: `/proc/stat`
- **RAM**: `/proc/meminfo`
- **CPU frequency**: `/sys/devices/system/cpu/cpufreq/policy*/cpuinfo_avg_freq`
- **CPU temps**: `/sys/class/hwwon`
- **GPU info**: `NVML` (requires CUDA to be installed on device!)
