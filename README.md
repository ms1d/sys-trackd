# sys-trackd

## About

Lightweight C system resource tracking daemon. Collects stats from machine, collates
it to JSON (or another standard output, not decided). Will personally use with waybar
but can ideally be reused anywhere.

## Primitves

- **CPU usage**: `/proc/stat`
- **CPU frequency**: `/sys/devices/system/cpu/cpufreq/policy*/cpuinfo_avg_freq`
- **CPU temps**: `/sys/class/hwwon`
- **RAM usage**: `/proc/meminfo`
- **GPU info**: `NVML` (requires CUDA to be installed on device!)
