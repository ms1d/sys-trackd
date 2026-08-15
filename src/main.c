#include <stdio.h>
#include "cpu_usage.h"


int main() {
	for(;;) {
		cpu_usage();
	}

	return 1;
}
