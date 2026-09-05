# sys-trackd

## About

Lightweight C system resource tracking daemon. Collects stats from machine, collates
it to JSON (or another standard output, not decided). Will personally use with waybar
but can ideally be reused anywhere.

## Features & Primitives

- **CPU usage**: `/proc/stat`
- **CPU frequency**: `/sys/devices/system/cpu/cpufreq/policy*/cpuinfo_avg_freq`
- **CPU temps**: `/sys/class/hwwon`
- **RAM usage**: `/proc/meminfo`

## Benchmarks

All runs were done on a Ryzen 7 8845HS (single threaded). Times do **NOT**
include daemon startup. `hyperfine` with 50k runs and no shell was used to record
this data. The significant variance in the data is due to the incredible short-lived
nature of each run.

| Name | Avg Time ± S.D. (us) | Range in Time (us) |
| ---- | -------------------- | ------------------ |
| cpu usage | 600.6 ± 138.1 | 322.6 ... 2162.2 |
| mem usage | 511.7 ± 138.9 | 313.7 ... 2708.4 |
| cpu temps | 583.9 ± 129.2 | 314.6 ... 2129.0 |
| cpu freqs | 616.4 ± 132.0 | 330.7 ... 2099.5 |

## Limitations

- No GPU monitoring (NVIDIA support is next)

- Single threaded execution (concurrency is planned)
