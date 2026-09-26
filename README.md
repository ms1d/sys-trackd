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

All runs were done on a Ryzen 7 8845HS (single threaded). Times do **NOT** include
daemon startup. `hyperfine` with 100k runs (+ 10k warmup runs) and no shell was
used to record this data. The significant variance in the data is due to the incredibly
short-lived nature of each run. The first table displays the results from omitting
`-DENABLE_CACHE` flag (default); the latter displays the results when it was introduced.

| Name | Avg Time ± S.D. (us) | Range in Time (us) |
| ---- | -------------------- | ------------------ |
| cpu usage | 682.1 ± 107.5 | 333.9 ... 2180.2 |
| mem usage | 640.1 ± 112.9 | 323.1 ... 2063.8 |
| cpu temps | 738.1 ± 104.6 | 378.9 ... 2386.2 |
| cpu freqs | 1116.8 ± 140.2 | 620.8 ... 3066.6 |

| Name | Avg Time ± S.D. (us) | Range in Time (us) | Avg. Time speedup |
| ---- | -------------------- | ------------------ | ----------------- |
| cpu usage | 674.9 ± 110.3 | 338.4 ... 2171.1 | 1.01x |
| mem usage | 626.2 ± 117.5 | 308.5 ... 2186.2 | 1.02x |
| cpu temps | 582.1 ± 139.3 | 313.1 ... 2614.7 | 1.27x |
| cpu freqs | 674.9 ± 111.1 | 337.8 ... 2132.8 | 1.65x |

Caching file descriptors had negligible impact on relatively simple metrics such
as CPU and RAM utilisation; CPU Temps and Freqs involve a much more aggressive filesystem
traversal process, and this cost adds upto nearly half a millisecond on average.
Caching allows the more expensive metrics to be collected just as quickly, if not
faster, than the others.

## Limitations

- No GPU monitoring (NVIDIA support is next)

- Single threaded execution (concurrency is planned)
