#include <stdio.h>
#include "cpu_usage.h"


int main() {
	for(;;) {
		printf("%d\n", cpu_usage());
	}

	return 1;
}
