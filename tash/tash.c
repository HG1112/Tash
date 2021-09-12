#include <stdio.h>
#include "mode.h"

int main(int argc, char** argv) {
	if (argc == 2) {
			batchmode(argv[1]);
	} else {
			interactive();
	}
	return 0;
}
