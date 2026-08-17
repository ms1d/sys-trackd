#pragma once
#include <unistd.h>


// Parses `start` upto the first `target` or the end of the buffer, whichever comes first
static inline const char *seek_char(const char* start, const int len, const char target) {
	const char *out = (char*)start;
	while ((out - start < len) && *(out++) != target);
	return (*(--out) != target) ? NULL : out;
}


// Parses `start` upto the first digit or the end of the buffer, whichever comes first
static inline const char *seek_digit(const char* start, const int len) {
	const char *out = (char*)start;
	while ((out - start < len) && (*out < '0' || *out > '9')) out++;
	return (*out < '0' || *out > '9') ? NULL : out;
}
