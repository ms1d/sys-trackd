#pragma once
#include <ctype.h>
#include <unistd.h>


// Parses `start` upto the first `target` or the end of the buffer, whichever comes first
static inline const char *seek_char(const char* start, const unsigned int len, const char target) {
	if (start == NULL || len == 0) return NULL;
	const char *out = (char*)start;
	while ((out - start < len) && *(out++) != target);
	return (*(--out) != target) ? NULL : out;
}

static inline const char *seek_not_char(const char* start, const unsigned int len, const char target) {
	if (start == NULL || len == 0) return NULL;
	const char *out = (char*)start;
	while ((out - start < len) && *(out++) == target);
	return (*(--out) == target) ? NULL : out;
}

static inline const char *seek_not_whitespace(const char* start, const unsigned int len) {
	if (start == NULL || len == 0) return NULL;
	const char *out = (char*)start;
	while ((out - start < len) && isspace((unsigned char)*(out++)));
	return (isspace((unsigned char)*(--out))) ? NULL : out;
}

static inline const char *seek_whitespace(const char* start, const unsigned int len) {
	if (start == NULL || len == 0) return NULL;
	const char *out = (char*)start;
	while ((out - start < len) && !isspace((unsigned char)*(out++)));
	return (!isspace((unsigned char)*(--out))) ? NULL : out;
}

// Parses `start` upto the first digit or the end of the buffer, whichever comes first
static inline const char *seek_digit(const char* start, const unsigned int len) {
	if (start == NULL || len == 0) return NULL;
	const char *out = (char*)start;
	while ((out - start < len) && (*out < '0' || *out > '9')) out++;
	return (out - start >= len) ? NULL : out;
}
